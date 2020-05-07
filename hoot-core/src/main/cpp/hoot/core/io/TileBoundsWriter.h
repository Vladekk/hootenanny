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

#ifndef TILE_BOUNDS_WRITER_H
#define TILE_BOUNDS_WRITER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

// GEOS
#include <geos/geom/Envelope.h>

namespace hoot
{

class TileBoundsWriter
{
public:

  /**
   * Writes boundary tiles to a GeoJSON output file
   *
   * @param tiles the collection of tiles to write
   * @param nodeCounts a collection of the number of node counts for each computed tile
   * @param outputPath the output file path
   * @param selectSingleRandomTile
   * @param randomSeed optional random number generator seed
   * @todo collapse with OSM writing method and refactor to TileFootprintWriter?
   * @todo refactor selectSingleRandomTile out?
   */
  static void writeTilesToGeoJson(const std::vector<std::vector<geos::geom::Envelope>>& tiles,
                                  const std::vector<std::vector<long>>& nodeCounts,
                                  const QString& outputPath,
                                  const QString& fileSource = "",
                                  const bool selectSingleRandomTile = false, int randomSeed = -1);

  /**
   * Writes boundary tiles to an OSM output file
   *
   * @param tiles the collection of tiles to write
   * @param nodeCounts a collection of the number of node counts for each computed tile
   * @param outputPath the output file path
   * @param selectSingleRandomTile
   * @param randomSeed optional random number generator seed
   * @todo collapse with GeoJSON writing method and refactor to TileFootprintWriter?
   * @todo refactor selectSingleRandomTile out?
   */
  static void writeTilesToOsm(
    const std::vector<std::vector<geos::geom::Envelope>>& tiles,
    const std::vector<std::vector<long>>& nodeCounts, const QString& outputPath,
    const bool selectSingleRandomTile = false, int randomSeed = -1);

private:

  /**
   * Convert boundary tiles to an OsmMap object
   *
   * @param tiles the collection of tiles to write
   * @param nodeCounts a collection of the number of node counts for each computed tile
   * @param randomTileIndex index of the random tile to use
   * @param selectSingleRandomTile
   */
  static OsmMapPtr _tilesToOsmMap(const std::vector<std::vector<geos::geom::Envelope>>& tiles,
    const std::vector<std::vector<long>>& nodeCounts,
    int randomTileIndex, const bool selectSingleRandomTile = false);
};

}

#endif // TILE_BOUNDS_WRITER_H
