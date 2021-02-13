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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef FRECHETSUBLINEMATCHER_H
#define FRECHETSUBLINEMATCHER_H

// hoot
#include <hoot/core/algorithms/subline-matching/SublineMatcher.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * A SublineMatcher based on the Frechet distance algorithm.
 */
class FrechetSublineMatcher : public SublineMatcher, public Configurable
{
public:
  static QString className() { return "hoot::FrechetSublineMatcher"; }

  FrechetSublineMatcher() = default;
  virtual ~FrechetSublineMatcher() = default;

  /**
   * @param maxRelevantDistance This value is set on a per match basis because it tends to vary
   *  based on the CE of the inputs. If set to -1 then the value is derived based on the CE of the
   *  input ways.
   */
  virtual WaySublineMatchString findMatch(const ConstOsmMapPtr& map, const ConstWayPtr& way1,
    const ConstWayPtr& way2, double& score, Meters maxRelevantDistance = -1) const override;

  virtual void setMaxRelevantAngle(Radians angle) override { _maxAngle = angle; }
  virtual void setMinSplitSize(Meters /*minSplitSize*/) override {}
  virtual void setHeadingDelta(Meters /*headingDelta*/) override {}

  virtual void setConfiguration(const Settings &conf);

  virtual QString getDescription() const
  { return "Matches lines based on the Frechet Distance algorithm"; }

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

private:

  Radians _maxAngle;
};

}
#endif // FRECHETSUBLINEMATCHER_H
