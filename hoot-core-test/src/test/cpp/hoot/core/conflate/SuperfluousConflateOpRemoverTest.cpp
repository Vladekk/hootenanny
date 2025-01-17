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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/polygon/BuildingMatchCreator.h>
#include <hoot/core/conflate/highway/HighwayMatchCreator.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatchCreator.h>
#include <hoot/core/conflate/network/NetworkMatchCreator.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class SuperfluousConflateOpRemoverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SuperfluousConflateOpRemoverTest);
  CPPUNIT_TEST(runOpFilter1Test);
  CPPUNIT_TEST(runOpFilter2Test);
  CPPUNIT_TEST(runOpFilter3Test);
  CPPUNIT_TEST(runOpFilter4Test);
  CPPUNIT_TEST(runOpFilterCombinedTest);
  CPPUNIT_TEST_SUITE_END();

public:

  SuperfluousConflateOpRemoverTest()
  {
    setResetType(ResetAllNoMatchFactory);
  }

  void runOpFilter1Test()
  { 
    TestUtils::runConflateOpReductionTest(
      QStringList(BuildingMatchCreator::className()), 2, 13, 11);
  }

  void runOpFilter2Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(HighwayMatchCreator::className()), 3, 14, 15);
  }

  void runOpFilter3Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(NetworkMatchCreator::className()), 3, 14, 15);
  }

  void runOpFilter4Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(PoiPolygonMatchCreator::className()), 1, 12, 9);
  }

  void runOpFilterCombinedTest()
  {
    QStringList matchCreators;
    matchCreators.append(BuildingMatchCreator::className());
    matchCreators.append(HighwayMatchCreator::className());
    matchCreators.append(NetworkMatchCreator::className());
    matchCreators.append(PoiPolygonMatchCreator::className());

    TestUtils::runConflateOpReductionTest(matchCreators, 4, 15, 17);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SuperfluousConflateOpRemoverTest, "quick");

}
