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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "CreatorDescription.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/CollectionRelationCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/LinearWaterwayCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/NonBuildingAreaCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/PowerLineCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>

namespace hoot
{

CreatorDescription::CreatorDescription() :
_experimental(false),
_baseFeatureType(BaseFeatureType::Unknown),
_geometryType(GeometryTypeCriterion::GeometryType::Unknown)
{
}

CreatorDescription::CreatorDescription(const QString& className, const QString& description,
                                       bool experimental) :
_experimental(experimental),
_className(className),
_description(description),
_baseFeatureType(BaseFeatureType::Unknown),
_geometryType(GeometryTypeCriterion::GeometryType::Unknown)
{
}

CreatorDescription::CreatorDescription(const QString& className, const QString& description,
                                       BaseFeatureType featureType, bool experimental) :
_experimental(experimental),
_className(className),
_description(description),
_baseFeatureType(featureType),
_geometryType(GeometryTypeCriterion::GeometryType::Unknown)
{
}

QString CreatorDescription::baseFeatureTypeToString(BaseFeatureType t)
{
  switch (t)
  {
    case POI:
      return "POI";
    case Highway:
      return "Road";
    case Building:
      return "Building";
    case Waterway:
      return "Waterway";
    case PoiPolygonPOI:
      return "Polygon Conflatable POI";
    case Polygon:
      return "Polygon";
    case Area:
      return "Area";
    case Railway:
      return "Railway";
    case PowerLine:
      return "Power Line";
    case Point:
      return "Point";
    case Line:
      return "Line";
    case CollectionRelation:
      return "Collection Relation";
    default:
      return "Unknown";
  }
}

CreatorDescription::BaseFeatureType CreatorDescription::stringToBaseFeatureType(QString s)
{
  s = s.toLower();
  if (0 == s.compare("poi"))
    return POI;
  else if (0 == s.compare("road"))
    return Highway;
  else if (0 == s.compare("building"))
    return Building;
  else if (0 == s.compare("waterway"))
    return Waterway;
  else if (0 == s.compare("polygon conflatable poi"))
    return PoiPolygonPOI;
  else if (0 == s.compare("polygon"))
    return Polygon;
  else if (0 == s.compare("area"))
    return Area;
  else if (0 == s.compare("railway"))
    return Railway;
  else if (0 == s.compare("powerline"))
    return PowerLine;
  else if (0 == s.compare("point"))
    return Point;
  else if (0 == s.compare("line"))
    return Line;
  else if (0 == s.compare("collectionrelation"))
    return CollectionRelation;
  else
    return Unknown;
}

CreatorDescription::FeatureCalcType CreatorDescription::getFeatureCalcType(BaseFeatureType t)
{
  switch (t)
  {
    case POI:
      return CalcTypeNone;
    case Highway:
      return CalcTypeLength;
    case Building:
      return CalcTypeArea;
    case Waterway:
      return CalcTypeLength;
    case PoiPolygonPOI:
      return CalcTypeNone;
    case Polygon:
      return CalcTypeArea;
    case Area:
      return CalcTypeArea;
    case Railway:
      return CalcTypeLength;
    case PowerLine:
      return CalcTypeLength;
    case Point:
      return CalcTypeNone;
    case Line:
      return CalcTypeLength;
    case CollectionRelation:
      return CalcTypeArea;
    default:
      return CalcTypeNone;
  }
}

std::shared_ptr<GeometryTypeCriterion> CreatorDescription::getElementCriterion(
  BaseFeatureType t, ConstOsmMapPtr map)
{
  switch (t)
  {
    case POI:
      return std::make_shared<PoiCriterion>();
    case Highway:
      return std::make_shared<HighwayCriterion>(map);
    case Building:
      return std::make_shared<BuildingCriterion>(map);
    case Waterway:
      return std::make_shared<LinearWaterwayCriterion>();
    case PoiPolygonPOI:
      return std::make_shared<PoiPolygonPoiCriterion>();
    case Polygon:
      return std::make_shared<PolygonCriterion>(map);
    case Area:
      return std::make_shared<NonBuildingAreaCriterion>(map);
    case Railway:
      return std::make_shared<RailwayCriterion>();
    case PowerLine:
      return std::make_shared<PowerLineCriterion>();
    case Point:
      return std::make_shared<PointCriterion>(map);
    case Line:
      return std::make_shared<LinearCriterion>();
    case CollectionRelation:
      return std::make_shared<CollectionRelationCriterion>();
    default:
      return std::shared_ptr<GeometryTypeCriterion>();
  }
}

QString CreatorDescription::getElementCriterionName(BaseFeatureType t)
{
  switch (t)
  {
    case POI:
      return PoiCriterion::className();
    case Highway:
      return HighwayCriterion::className();
    case Building:
      return BuildingCriterion::className();
    case Waterway:
      return LinearWaterwayCriterion::className();
    case PoiPolygonPOI:
      return PoiPolygonPoiCriterion::className();
    case Polygon:
      return PolygonCriterion::className();
    case Area:
      return AreaCriterion::className();
    case Railway:
      return RailwayCriterion::className();
    case PowerLine:
      return PowerLineCriterion::className();
    case Point:
      return PointCriterion::className();
    case Line:
      return LinearCriterion::className();
    case CollectionRelation:
      return CollectionRelationCriterion::className();
    default:
      return "";
  }
}

QString CreatorDescription::toString() const
{
  return _className + ";" + baseFeatureTypeToString(_baseFeatureType);
}

}
