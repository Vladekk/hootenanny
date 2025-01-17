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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "PoiPolygonTagIgnoreListReader.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/TagListReader.h>

namespace hoot
{

PoiPolygonTagIgnoreListReader::PoiPolygonTagIgnoreListReader()
{
  LOG_DEBUG("Reading ignore lists...");
  _poiTagIgnoreList =
    TagListReader::readList(ConfigOptions().getPoiPolygonPoiIgnoreTagsFile());
  LOG_VARD(_poiTagIgnoreList.size());
  _polyTagIgnoreList =
    TagListReader::readList(ConfigOptions().getPoiPolygonPolyIgnoreTagsFile());
  LOG_VARD(_polyTagIgnoreList.size());
}

PoiPolygonTagIgnoreListReader& PoiPolygonTagIgnoreListReader::getInstance()
{
  //  Local static singleton instance
  static PoiPolygonTagIgnoreListReader instance;
  return instance;
}

}
