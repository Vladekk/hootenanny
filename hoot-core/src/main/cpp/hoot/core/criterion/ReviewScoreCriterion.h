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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef REVIEW_SCORE_CRITERION_H
#define REVIEW_SCORE_CRITERION_H

// Hoot
#include <hoot/core/criterion/ReviewRelationCriterion.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Allows for filtering reviews by score
 */
class ReviewScoreCriterion : public ReviewRelationCriterion, public Configurable
{
public:

  static QString className() { return "hoot::ReviewScoreCriterion"; }

  ReviewScoreCriterion();
  ~ReviewScoreCriterion() = default;

  bool isSatisfied(const ConstElementPtr& e) const override;

  void setConfiguration(const Settings& conf) override;

  ElementCriterionPtr clone() override
  { return ElementCriterionPtr(new ReviewScoreCriterion()); }

  QString getDescription() const override { return "Allows for filtering reviews by score"; }

  void setMinScoreThreshold(const double threshold);
  void setMaxScoreThreshold(const double threshold);
  void setInvertThresholding(const bool invert) { _invertThresholding = invert; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

private:

  double _minScoreThreshold;
  double _maxScoreThreshold;
  bool _invertThresholding;
};

}

#endif // REVIEW_SCORE_CRITERION_H
