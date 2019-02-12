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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_UTILS_H
#define OSM_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/elements/ConstElementVisitor.h>

// Boost Includes
#include <boost/shared_ptr.hpp>

// GEOS
#include <geos/geom/Coordinate.h>

// Qt
#include <QString>

namespace hoot
{

class Node;
class Status;

/**
  Utilities for use with the OSM data model
  */
class OsmUtils
{
public:

  /**
    Prints a collection of nodes for debugging purposes (couldn't find a way to do this with the
    debug logger).

    @param nodeCollectionName
    @param nodes
    */
  static void printNodes(const QString nodeCollectionName,
                         const QList<boost::shared_ptr<const Node>>& nodes);

  /**
    Retrieves a collection of node ID's for a collection of nodes

    @param nodes a collection of nodes
    @return a collection of node ID's
    */
  static const QList<long> nodesToNodeIds(const QList<boost::shared_ptr<const Node>>& nodes);

  /**
    Retrieves a collection of nodes given a collection of node ID's

    @param nodeIds a collection of node ID's
    @param map the map owning the nodes with the given ID's
    @return a collection of nodes
    */
  static QList<boost::shared_ptr<const Node>> nodeIdsToNodes(const QList<long>& nodeIds,
                                                             boost::shared_ptr<const OsmMap> map);

  /**
    Converts a OSM node to a coordinate

    @param node the node to convert
    @returns a coordinate
    */
  static geos::geom::Coordinate nodeToCoord(boost::shared_ptr<const Node> node);

  /**
    Converts OSM nodes to a coordinates

    @param nodes the nodes to convert
    @returns coordinates
    */
  static QList<geos::geom::Coordinate> nodesToCoords(
    const QList<boost::shared_ptr<const Node> >& nodes);

  /**
    Converts a coordinate to an OSM node

    @param coord the coordinate to convert
    @param map the map owning the node to be created
    @returns a node
    */
  static boost::shared_ptr<const Node> coordToNode(const geos::geom::Coordinate& coord,
                                                   boost::shared_ptr<const OsmMap> map);

  /**
    Converts coordinates to OSM nodes

    @param coords the coordinates to convert
    @param map the map owning the nodes to be created
    @returns nodes
    */
  static QList<boost::shared_ptr<const Node> > coordsToNodes(
    const QList<geos::geom::Coordinate>& coords, boost::shared_ptr<const OsmMap> map);

  /**
    Converts a 64-bit unsigned int timestamp (seconds from epoch) to a QString (utc zulu)

    @param timestamp quint64 time encoding in seconds from the epoch (1970-01-01T00:00:00Z)
    */
  static QString toTimeString(quint64 timestamp);

  /**
    Converts a utc zulu timestamp to time since the epoch in seconds.

    @param timestamp in utc zulu string to be convered to seconds from the epoch (1970-01-01 00:00:00)
    */
  static quint64 fromTimeString(QString timestamp);

  /**
   * Returns a time string for the current time
   *
   * @return
   */
  static QString currentTimeAsString();

  /**
   * Determines whether a map contains a minimum or a fixed amount of elements matching the criterion type
   * Only objects of type ElementCriterion are allowed, all others will return false
   *
   * @param map the map to examine
   * @param minCount the minmal count of elements required (if exactCount == false)
   * @param exactCount if true, the count must be exactly minCount
   * @return true if the map meets the specified criteria; false otherwise
   */
  template<class C> static bool contains(ConstOsmMapPtr map, int minCount = 1, bool exactCount = false)
  {
    if(!std::is_base_of<ElementCriterion,C>::value) return false;

    const long count =
      (long)FilteredVisitor::getStat(
        ElementCriterionPtr(new C()),
        ConstElementVisitorPtr(new ElementCountVisitor()),
        map);
    LOG_VART(count);
    return exactCount ? (count == minCount) : (count >= minCount);
  }

  /**
   *
   *
   * @param relation
   * @param map
   * @return
   */
  static QString getDetailedRelationString(ConstRelationPtr& relation, const ConstOsmMapPtr& map);
};

}

#endif // OSM_UTILS_H
