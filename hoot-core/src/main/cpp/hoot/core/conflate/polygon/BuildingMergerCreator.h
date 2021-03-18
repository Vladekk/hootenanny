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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef BUILDINGMERGECREATOR_H
#define BUILDINGMERGECREATOR_H

// hoot
#include <hoot/core/conflate/merging/MergerCreator.h>

namespace hoot
{

class BuildingMergerCreator : public MergerCreator
{
public:

  static QString className() { return "hoot::BuildingMergerCreator"; }

  BuildingMergerCreator() = default;
  ~BuildingMergerCreator() = default;

  /**
   * If all the matches are BuildingMatches, a single BuildingMerge will be created and returned.
   */
  bool createMergers(const MatchSet& matches, std::vector<MergerPtr>& mergers) const override;

  std::vector<CreatorDescription> getAllCreators() const override;

  bool isConflicting(const ConstOsmMapPtr& map, ConstMatchPtr m1, ConstMatchPtr m2,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const override;
};

}

#endif // BUILDINGMERGECREATOR_H
