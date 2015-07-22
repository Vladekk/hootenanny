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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ScriptTranslatorFactory.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/io/ScriptTranslator.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

ScriptTranslatorFactory ScriptTranslatorFactory::_theInstance;

ScriptTranslatorFactory::ScriptTranslatorFactory()
{
}

bool CompareSt(ScriptTranslator* st1, ScriptTranslator* st2)
{
  return st1->order() < st2->order();
}

ScriptTranslator* ScriptTranslatorFactory::createTranslator(QString scriptPath)
{
//  LOG_DEBUG("script path before: " + scriptPath);
//  const QString hootHome = QString(getenv("HOOT_HOME"));
//  //TODO: should HOOT_HOME be verified not to be an empty string here?
//  if (!scriptPath.startsWith(hootHome))
//  {
//    scriptPath = hootHome + "/" + scriptPath;
//  }
//  LOG_DEBUG("script path after: " + scriptPath);

  _init();

  vector<ScriptTranslator*> st;
  for (size_t i = 0; i < _translators.size(); ++i)
  {
    st.push_back(Factory::getInstance().constructObject<ScriptTranslator>(_translators[i]));
  }

  sort(st.begin(), st.end(), CompareSt);

  ScriptTranslator* result = 0;
  for (size_t i = 0; i < st.size(); ++i)
  {
    try
    {
      st[i]->setScript(scriptPath);
      if (result == 0 && st[i]->isValidScript())
      {
        result = st[i];
      }
      else
      {
        delete st[i];
      }
    }
    catch (...)
    {
      LOG_WARN("isValidScript shouldn't throw an exception.");
      delete st[i];
    }
  }

  if (result == 0)
  {
    throw HootException("Unable to find an appropriate scripting language for: " + scriptPath);
  }

  return result;
}

void ScriptTranslatorFactory::_init()
{
  if (_translators.size() == 0)
  {
    _translators = Factory::getInstance().getObjectNamesByBase(ScriptTranslator::className());
  }
}

}

