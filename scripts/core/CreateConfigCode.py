#!/usr/bin/python

"""
Creates C++ code for the available configuration options. This forces the 
configuration strings to be checked at compile time.
"""

import re
import sys
import json
import textwrap
import datetime

if (len(sys.argv) != 4):
    print "Usage:"
    print sys.argv[0] + " (config file) (output header) (output defaults header)"
    sys.exit(-1)

fn = sys.argv[1]
headerFn = sys.argv[2]
defaultsHeaderFn = sys.argv[3]

notice = """/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2012-{XXXX} Maxar (http://www.maxar.com/)
 */

////
// This file is automatically generated. Please do not modify the file 
// directly.
////
"""

notice = notice.replace("{XXXX}", datetime.date.today().strftime("%Y"));

def escapeString(s):
    return s.encode('string-escape').replace("\"", "\\\"");

def stripDefaultValue(s):
    m = re.search("\\* Default Value: `(.*)`", s)
    if m:
        return m.group(1)
    else:
        raise Exception("Improperly formatted default value: " + s)

def stripDouble(prefix, s):
    m = re.search("\\* " + prefix + ": `(-?\\d+\\.?\\d*)`", s)
    if m:
        return float(m.group(1))
    else:
        raise Exception("Improperly-formatted floating-point value for " + prefix + ": " + s)

def stripInt(prefix, s):
    m = re.search("\\* " + prefix + ": `(-?\\d+)`", s)
    if m:
        return int(m.group(1))
    else:
        raise Exception("Improperly-formatted integer value for " + prefix + ": " + s)

def loadAsciiDoc(fn):
    result = {}
    section = ''
    element = {}
    inBody = False
    inList = False
    lines = open(fn).readlines();
    ln = 0
    for l in lines:
        ln = ln + 1
        # remove trailing white space
        ls = l.rstrip();

        # comment
        if (ls.strip().startswith("//")):
            pass
        # New section
        elif (ls.startswith("=== ")):
            section = ls.replace("=== ", "")
            element = {}
            result[section] = element
            inBody = False
            inList = False
        elif (ls.startswith("* Data Type: ")):
            element['type'] = ls.replace("* Data Type: ", "");
            inBody = False
        elif (l.startswith("* Default Value:")):
            if (element['type'] == 'list'):
                element['default'] = []
                inList = True
            elif (element['type'] == 'bool'):
                d = stripDefaultValue(l)
                if d == 'true': element['default'] = True
                elif d == 'false': element['default'] = False
                else: raise Exception("Unexpected bool default value in " + section)
                inBody = True
                inList = False
            else:
                element['default'] = stripDefaultValue(l)
                inBody = True
                inList = False
  	elif (ls.startswith("* Minimum: ")):
            if (element['type'] == 'int'):
                element['minimum'] = stripInt("Minimum", ls)
            elif (element['type'] == 'double'):
                element['minimum'] = stripDouble("Minimum", ls)
            else: 
                raise Exception("Minimum value specified for unsupported type " + element['type'] + 
                    " in " + section)
            if ( 'maximum' in element):
                if (element['maximum'] < element['minimum']):
                    raise Exception("Min <= Max bounds relationship not valid in " + section + 
                        "; min = " + str(element['minimum']) + ", max = " + str(element['maximum']))
                elif ('default' in element):
                    default = 0;
                    if ( element['type'] == 'int' ):
                        default = int(element['default'])
                    else:
                        default = float(element['default'])
                    if ((default < element['minimum']) 
                        or (default > element['maximum'])):
                        raise Exception("Invalid relationship between min/default/max in " 
                            + section + ": min=" + str(element['minimum']) + ", default=" 
                            + str(default) + ", max=" + str(element['maximum']))
        elif (ls.startswith("* Maximum: ")):
            if (element['type'] == 'int'):
                element['maximum'] = stripInt("Maximum", ls)
            elif (element['type'] == 'double'):
                element['maximum'] = stripDouble("Maximum", ls)
            else: 
                raise Exception("Maximum value specified for unsupported type " + element['type'] + 
                    " in " + section)
            if ( 'minimum' in element):
                if (element['maximum'] < element['minimum']):
                    raise Exception("Min <= Max bounds relationship not valid in " + section +
                        "; min = " + str(element['minimum']) + ", max = " + str(element['maximum']))
                elif ('default' in element):
                    default = 0;
                    if ( element['type'] == 'int' ):
                        default = int(element['default'])
                    else:
                        default = float(element['default'])
                    if ((default < element['minimum']) or 
                        (default > element['maximum'])):
                        raise Exception("Invalid relationship between min/default/max in " + section + 
                            ": min=" + str(element['minimum']) + ", default=" + str(default) + 
                            ", max=" + str(element['maximum']))
        elif inList:
            if ls.startswith('** '):
                element['default'].append(ls.split('`')[1]);
            elif ls == '':
                inList = False
                inBody = True
            else:
                raise Exception("Expected a blank line after the default list. In: " + section)
        elif inBody:
            if 'description' not in element:
                if (l != ''):
                    element['description'] = l
            else:
                element['description'] = element['description'] + l
        elif ls == '':
            pass
        else:
            raise Exception("unexpected line (%d) in: %s line: %s" % (ln, section, l))

    for k, v in result.iteritems():
        v['description'] = v['description'].strip();

    return result

if fn.endswith(".asciidoc"):
    c = loadAsciiDoc(fn)
else:
    c = json.loads("".join(open(fn).readlines()))

def cppTypeMap(s):
    if s == 'string':
        return 'QString';
    if s == 'long' or s == 'int' or s == 'double' or s == 'bool':
        return s
    if s == 'list':
    	  return 'QStringList';
    raise Exception("Expected a type of string, long, int, bool, double, or list.")

def toCamelCase(s):
    l = list(s)
    l[0] = l[0].upper()
    for i in range(1, len(l)):
        if l[i - 1] == '.':
            l[i] = l[i].upper()

    s = "".join(l)
    return s.replace('.', '')

def toComment(indent, s):
    lines = s.split('\n')
    l = []
    for i in lines:
        # keep empty lines
        if i.strip() != '':
            l = l + textwrap.wrap(i, 95 - indent)
        else:
            l.append(i)

    for i in range(0, len(l)):
        l[i] = " " * indent + "* " + l[i]

    return "\n".join(l)

def createHeader(c):
    result = notice
    result += """

#ifndef __CONFIG_OPTIONS_H__
#define __CONFIG_OPTIONS_H__

#include <hoot/core/util/Settings.h>

namespace hoot
{

/**
 * This class provides a compile time set of configuration option keys and
 * default values.
 */
class ConfigOptions
{
public:
  ConfigOptions() : _s(conf()) {}
  ConfigOptions(const Settings& s) : _s(s) {}
"""
    keys = list(c.keys())
    keys.sort()
    for k in keys:
        v = c[k]
        valueString = v['default']
        if v['type'] == 'string':
            valueString = 'QString::fromUtf8("%s")' % escapeString(valueString)
        if v['type'] == 'bool':
            if valueString:
                valueString = 'true'
            else:
                valueString = 'false'
        if v['type'] == 'list':
          valueString = 'QString::fromUtf8("' + escapeString(';'.join(valueString)) + '").trimmed().split(";")'	
        cc = toCamelCase(k)
        result += """
  /**
%s
   */
""" % (toComment(3, v['description']))

        if ( ('minimum' in v) and ('maximum' in v) ):
            result += """     
  %s get%s() const { return _s.get%s(get%sKey(), get%sDefaultValue(), get%sMinimum(), get%sMaximum()); }
""" % ( cppTypeMap(v['type']), cc, toCamelCase(v['type']), cc, cc, cc, cc )
        else:
            result += """
  %s get%s() const { return _s.get%s(get%sKey(), get%sDefaultValue()); }
""" % ( cppTypeMap(v['type']), cc, toCamelCase(v['type']), cc, cc )

        result += """
  static QString get%sKey() { return "%s"; }
  static QString get%sDescription() { return "%s"; }
""" % (
            cc, k, 
            cc, escapeString(v['description']))

        if (v['type'] == 'double') and "$" in valueString:
            result += '  %s get%sDefaultValue() const { return _s.getDoubleValue("%s"); }\n' \
                % (cppTypeMap(v['type']), cc, valueString)
            pass
        else:
            result += "  static %s get%sDefaultValue() { return %s; }\n" % \
                (cppTypeMap(v['type']), cc, valueString)

        if ( ('minimum' in v) and ('maximum' in v) ):
            result += "  static %s get%sMinimum() { return %s; }\n" % \
                (cppTypeMap(v['type']), cc, str(v['minimum']))
            result += "  static %s get%sMaximum() { return %s; }\n" % \
                (cppTypeMap(v['type']), cc, str(v['maximum']))

    result += """
  static void populateDefaults(Settings& s)
  {
"""

    for k in keys:
        v = c[k]
        valueString = v['default']
        if v['type'] == 'string':
            valueString = 'QString::fromUtf8("%s")' % escapeString(valueString)
        elif v['type'] == 'bool':
            if valueString:
                valueString = 'true'
            else:
                valueString = 'false'
        elif v['type'] == 'list':
          valueString = 'QString::fromUtf8("' + escapeString(';'.join(valueString)) + '").trimmed().split(";")'
        else:
          valueString = '"' + str(valueString) + '"'

        if (valueString != ''):
            result += '    s.set(QString::fromUtf8("%s"), %s);\n' % (k, valueString)
    result += "  }\n"

    result += """

private:
  const Settings& _s;
};

}

#endif // __CONFIG_OPTIONS__
"""

    return result

def createDefaultsHeader(c):
    result = notice
    result += """

#ifndef __CONFIG_DEFAULTS_H__
#define __CONFIG_DEFAULTS_H__

namespace hoot
{

/**
 * This class provides a compile time set of default configuration option keys 
 * and default values.
 */
class ConfigDefaults
{
public:
"""
    keys = list(c.keys())
    keys.sort()

    result += """
  static void populateDefaults(Settings& s)
  {
"""

    for k in keys:
        v = c[k]
        valueString = v['default']
        if v['type'] == 'string':
            valueString = 'QString::fromUtf8("%s")' % escapeString(valueString)
        elif v['type'] == 'bool':
            if valueString:
                valueString = 'true'
            else:
                valueString = 'false'
        elif v['type'] == 'list':
          valueString = 'QString::fromUtf8("' + escapeString(';'.join(valueString)) + '").trimmed().split(";")'
        else:
          valueString = '"' + str(valueString) + '"'

        if (valueString != ''):
            result += '    s.set(QString::fromUtf8("%s"), %s);\n' % (k, valueString)
    result += "  }\n"

    result += """
};

}

#endif // __CONFIG_DEFAULTS_H__
"""

    return result

# Why two files? We could put it in one. This makes compile time a bit faster
# and also avoids a circular dependency. See ticket #4703 for details.
open(headerFn, 'w').write(createHeader(c))
open(defaultsHeaderFn, 'w').write(createDefaultsHeader(c))

