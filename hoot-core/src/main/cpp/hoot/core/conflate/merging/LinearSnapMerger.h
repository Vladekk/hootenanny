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
#ifndef LINEAR_SNAP_MERGER_H
#define LINEAR_SNAP_MERGER_H

// Hoot
#include <hoot/core/conflate/merging/LinearMergerAbstract.h>

namespace hoot
{

class WaySublineCollection;

/**
 * Merges linear geometries and tags by snapping the secondary geometry to the reference geometry
 *
 * Note that this was originally written specifically for roads, but now is used by several linear
 * script routines, including railway and river.
 */
class LinearSnapMerger : public LinearMergerAbstract
{

public:

  static QString className() { return "hoot::LinearSnapMerger"; }

  LinearSnapMerger();
  LinearSnapMerger(
    const std::set<std::pair<ElementId, ElementId>>& pairs,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher);
  virtual ~LinearSnapMerger() = default;

  virtual QString getDescription() const override
  { return "Merges linear features by snapping geometries"; }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

protected:

  // When roads are split and the pieces stored in multilinestring relations, this decides if the
  // tags get removed from the constituent way members.
  bool _removeTagsFromWayMembers;

  // Determines whether we add a custom tag to any multilinestring relation created during merging.
  // This is useful for getting rid of them later, if necessary.
  bool _markAddedMultilineStringRelations;

  virtual bool _mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
                          std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  /*
   * Finds a matching subline between two elements with the configured subline matcher
   */
  virtual WaySublineMatchString _matchSubline(OsmMapPtr map, ElementPtr e1, ElementPtr e2);

private:

  // for white box testing
  friend class LinearSnapMergerTest;

  static int logWarnCount;

  static const bool WRITE_DETAILED_DEBUG_MAPS;

  /*
   * Snap the ends of snapee that match with either end point of middle to snapTo's end points.
   */
  void _snapEnds(const OsmMapPtr& map, ElementPtr snapee, ElementPtr snapTo) const;
  void _snapEnds(const OsmMapPtr& map, WayPtr snapee, WayPtr middle, WayPtr snapTo) const;
  void _snapEnd(const OsmMapPtr& map, WayPtr snapee, NodePtr replacedNode,
                NodePtr replacementNode) const;

  /*
   * Splits the splitee up into a match element and a scrap element. All the tags are assigned
   * appropriately and the match and scrap are added to the replaced list and added to the map.
   * The original elements are deleted.
   */
  void _splitElement(const OsmMapPtr& map, const WaySublineCollection& s,
                     const std::vector<bool>& reverse,
                     std::vector<std::pair<ElementId, ElementId>>& replaced,
                     const ConstElementPtr& splitee, ElementPtr& match, ElementPtr& scrap) const;

  void _updateScrapParent(const OsmMapPtr& map, long id, const ElementPtr& scrap);
};

using LinearSnapMergerPtr = std::shared_ptr<LinearSnapMerger>;

}

#endif // LINEAR_SNAP_MERGER_H