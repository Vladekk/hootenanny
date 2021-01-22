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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef RELATION_WITH_POWER_LINE_MEMBERS_CRITERION_H
#define RELATION_WITH_POWER_LINE_MEMBERS_CRITERION_H

// Hoot
#include <hoot/core/criterion/RelationWithMembersOfTypeCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/PowerLineCriterion.h>

namespace hoot
{

/**
 * Identifies relations having power line members
 */
class RelationWithPowerLineMembersCriterion : public RelationWithMembersOfTypeCriterion
{
public:

  static std::string className() { return "hoot::RelationWithPowerLineMembersCriterion"; }

  RelationWithPowerLineMembersCriterion();
  virtual ~RelationWithPowerLineMembersCriterion() = default;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new RelationWithPowerLineMembersCriterion()); }

  virtual QString getCriterion() const override
  { return QString::fromStdString(PowerLineCriterion::className()); }

  virtual QString getDescription() const
  { return "Identifies relations with power line members"; }

  virtual GeometryType getGeometryType() const { return PowerLineCriterion().getGeometryType(); }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }
};

}
#endif // RELATION_WITH_POWER_LINE_MEMBERS_CRITERION_H
