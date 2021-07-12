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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef WAY_NODE_COUNT_CRITERION_H
#define WAY_NODE_COUNT_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/NumericComparisonType.h>

namespace hoot
{

class WayNodeCountCriterion : public ElementCriterion
{
public:

  static QString className() { return "hoot::WayNodeCountCriterion"; }

  WayNodeCountCriterion();
  WayNodeCountCriterion(
    const int comparisonCount, const NumericComparisonType& numericComparisonType);
  ~WayNodeCountCriterion() = default;

  /**
   * @see ElementCriterion
   */
  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<WayNodeCountCriterion>(); }

  QString getDescription() const override
  { return "Identifies ways that meet a node count threshold"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }

protected:

  int _comparisonCount;
  NumericComparisonType _numericComparisonType;
};

}

#endif // WAY_NODE_COUNT_CRITERION_H
