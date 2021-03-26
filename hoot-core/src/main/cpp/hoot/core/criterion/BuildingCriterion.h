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
#ifndef BUILDINGCRITERION_H
#define BUILDINGCRITERION_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>

namespace hoot
{

/**
 * A criterion to identify buildings
 */
class BuildingCriterion : public ConflatableElementCriterion, public ConstOsmMapConsumer
{
public:

  static QString className() { return "hoot::BuildingCriterion"; }

  BuildingCriterion() = default;
  BuildingCriterion(ConstOsmMapPtr map) : _map(map) { }
  ~BuildingCriterion() = default;

  bool isParentABuilding(ElementId eid) const;

  bool isSatisfied(const ConstElementPtr& e) const override;

  bool isSatisfied(const Tags& tags, const ElementType& elementType) const;

  GeometryType getGeometryType() const override { return GeometryType::Polygon; }

  void setOsmMap(const OsmMap* map) override { _map = map->shared_from_this(); }

  ElementCriterionPtr clone() override { return ElementCriterionPtr(new BuildingCriterion(_map)); }

  QString getDescription() const override { return "Identifies buildings"; }

  QString getName() const override { return className(); }

  bool supportsSpecificConflation() const override { return true; }

  QString getClassName() const override { return className(); }

  QStringList getChildCriteria() const override;

private:

  ConstOsmMapPtr _map;
};

}

#endif // BUILDINGCRITERION_H
