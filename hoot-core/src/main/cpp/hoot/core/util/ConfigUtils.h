/*
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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CONFIGUTILS_H
#define CONFIGUTILS_H

namespace hoot
{

/**
 * Utilities for dealing with hoot config options
 */
class ConfigUtils
{

public:

  /**
   * Determines if any of the filter by bounds options are enabled
   *
   * @return true if any of the bounds options are enabled
   */
  static bool boundsOptionEnabled();

  /**
   * Removes ApiTruncateTagVisitor from conflate ops if the corresponding config option is
   * activated
   */
  static void checkForTagValueTruncationOverride();
};

}

#endif // CONFIGUTILS_H
