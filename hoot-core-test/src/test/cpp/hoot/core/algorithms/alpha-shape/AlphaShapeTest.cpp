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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/alpha-shape/AlphaShape.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>

// Standard
#include <fstream>
#include <list>
#include <cassert>

// Tgs
#include <tgs/Statistics/Random.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class AlphaShapeTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AlphaShapeTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runRandomTest);
  CPPUNIT_TEST_SUITE_END();

public:

  AlphaShapeTest()
    : HootTestFixture(UNUSED_PATH,
                      "test-output/algorithms/alpha-shape/")
  {
    setResetType(ResetBasic);
  }

  void createDonut(vector<pair<double, double>>& p, double innerRadius, double outerRadius,
                   double centerX, double centerY, int count)
  {
    for (int i = 0; i < count; i++)
    {
      double x = Tgs::Random::instance()->generateUniform() * outerRadius * 2.0 - outerRadius;
      double y = Tgs::Random::instance()->generateUniform() * outerRadius * 2.0 - outerRadius;
      double r = sqrt(x * x + y * y);
      if (r >= innerRadius && r <= outerRadius)
      {
        p.push_back(pair<double, double>(centerX + x, centerY + y));
      }
    }
  }

  void runRandomTest()
  {
    vector<pair<double, double>> points;

    createDonut(points, 0, 35, 10, 0, 250);
    //createDonut(points, 15, 20, 20, 0, 2000);

    AlphaShape uut(16.0);
    uut.insert(points);

    OsmMapPtr map = uut._toOsmMap();

    const WayMap ways = map->getWays();
    for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
    {
      const WayPtr& w = it->second;
      w->setTag("highway", "motorway");
    }

    for (size_t i = 0; i < points.size(); i++)
    {
      NodePtr n(new Node(Status::Invalid, map->createNextNodeId(), points[i].first, points[i].second, -1));
      map->addNode(n);
    }

    OsmXmlWriter writer;
    writer.write(map, _outputPath + "AlphaDonut.osm");

    std::shared_ptr<Geometry> g = uut.toGeometry();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3241.5, g->getArea(), 0.1);
  }

  void runTest()
  {
    vector<pair<double, double>> points;
    points.push_back(pair<double, double>(0.0, 0.0));
    points.push_back(pair<double, double>(0.0, 1.0));
    points.push_back(pair<double, double>(1.0, 1.0));
    points.push_back(pair<double, double>(1.0, 0.0));

    points.push_back(pair<double, double>(0.0, 0.5));
    points.push_back(pair<double, double>(0.5, 1.0));
    points.push_back(pair<double, double>(1.0, 0.5));
    points.push_back(pair<double, double>(0.5, 0.0));

    points.push_back(pair<double, double>(0.2, 0.2));
    points.push_back(pair<double, double>(0.2, 0.8));
    points.push_back(pair<double, double>(0.8, 0.8));
    points.push_back(pair<double, double>(0.8, 0.2));

    AlphaShape uut(0.61);

    uut.insert(points);

    OsmXmlWriter writer;
    writer.write(uut._toOsmMap(), _outputPath + "AlphaMap.osm");

    std::shared_ptr<Geometry> g = uut.toGeometry();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.64, g->getArea(), 0.001);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AlphaShapeTest, "quick");
}
