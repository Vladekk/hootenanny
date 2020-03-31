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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef COLLECTION_RELATION_MERGER_H
#define COLLECTION_RELATION_MERGER_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/OsmMapConsumer.h>

namespace hoot
{

/**
 * Merges collection relations e.g. routes, administrative boundaries, etc.
 */
class CollectionRelationMerger : public OsmMapConsumer
{
public:

  CollectionRelationMerger();

  /**
   * Merges two collection relations
   *
   * @param elementId1 ID of the first relation to merger
   * @param elementId2 ID of the second relation to merger
   */
  void merge(const ElementId& elementId1, const ElementId& elementId2);

  /**
   * @see OsmMapConsumer
   */
  virtual void setOsmMap(OsmMap* map) { _map = map->shared_from_this(); }

  void setIgnoreIds(bool ignore) { _ignoreIds = ignore; }

private:

  OsmMapPtr _map;

  // TODO: make configurable
  bool _ignoreIds;

  void _mergeMembers(RelationPtr replacingRelation, RelationPtr relationBeingReplaced);
  void _mergeMembersIgnoreIds(RelationPtr replacingRelation, RelationPtr relationBeingReplaced);
};

}

#endif // COLLECTION_RELATION_MERGER_H
