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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef NODETOWAYMAP_H
#define NODETOWAYMAP_H

// Standard
#include <map>
#include <memory>
#include <set>
#include <string>

namespace hoot
{

class OsmMap;
class Way;

/**
 * Traverses an OsmMap and creates a map from node id to a set of way IDs. This is handy when
 * trying to find intersections.
 */
class NodeToWayMap : public std::map<long, std::set<long>>
{
public:

  static QString className() { return "hoot::NodeToWayMap"; }

  NodeToWayMap(const OsmMap& map);
  virtual ~NodeToWayMap() = default;

  void addWay(const std::shared_ptr<const Way>& w);

  /**
   * Returns all the ways that use the given node ID. If the given node ID isn't found an empty
   * set is returned.
   */
  const std::set<long>& getWaysByNode(long nid) const;

  /**
   * This function assumes that the nodes in the way haven't changed since it was last added.
   */
  void removeWay(const std::shared_ptr<const Way>& w);

  bool validate(const OsmMap& map);

private:

  static int logWarnCount;

  std::set<long> _emptySet;
};

}

#endif // NODETOWAYMAP_H
