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
 * @copyright Copyright (C) 2012, 2013, 2014, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/elements/MapProjector.h>
using namespace hoot::pb;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include <zlib.h>

using namespace std;

namespace hoot
{

class OsmPbfReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmPbfReaderTest);
  CPPUNIT_TEST(runOffsetsTest);
  CPPUNIT_TEST(runReadIncrementalTest);
  CPPUNIT_TEST(runReadNodeTest);
  CPPUNIT_TEST(runReadRelationTest);
  CPPUNIT_TEST(runReadWayTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST(runToyRelationTest);
  CPPUNIT_TEST(runIsSupportedUrlExistsTest);
  CPPUNIT_TEST(runIsSupportedUrlDoesntExistTest);
  CPPUNIT_TEST(runIsSupportedUrlBadExtensionTest);
  CPPUNIT_TEST(runIsSupportedUrlIsDirTest);
  CPPUNIT_TEST(runOpenUrlExistsTest);
  CPPUNIT_TEST(runOpenUrlDoesntExistTest);
  CPPUNIT_TEST(runReadMapTest);
  CPPUNIT_TEST(runFactoryReadMapTest);
  CPPUNIT_TEST(runReadMapPartialTest);
  CPPUNIT_TEST(runReadMapPartialMultipleBlobsTest);
  CPPUNIT_TEST(runHasMoreElementsTest);
  CPPUNIT_TEST(runReadNextElementTest);
  CPPUNIT_TEST(runReadSortTypeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmPbfReaderTest()
    : HootTestFixture("test-files/io/OsmPbfReaderTest/",
                      "test-output/io/OsmPbfReaderTest/")
  {
    setResetType(ResetAll);
  }

  void runOffsetsTest()
  {
    OsmPbfReader uut(false);
    fstream input(_inputPath.toStdString() + "SmallSplits.pbf", ios::in | ios::binary);
    OsmMapPtr map(new OsmMap());

    vector<OsmPbfReader::BlobLocation> v = uut.loadOsmDataBlobOffsets(input);

    CPPUNIT_ASSERT_EQUAL((size_t)12, v.size());

    CPPUNIT_ASSERT_EQUAL(1141l, v[0].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1107l, v[0].blobSize);
    CPPUNIT_ASSERT_EQUAL(2264l, v[1].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1107l, v[1].blobSize);
    CPPUNIT_ASSERT_EQUAL(3387l, v[2].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1107l, v[2].blobSize);
    CPPUNIT_ASSERT_EQUAL(4510l, v[3].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1107l, v[3].blobSize);
    CPPUNIT_ASSERT_EQUAL(5633l, v[4].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1107l, v[4].blobSize);
    CPPUNIT_ASSERT_EQUAL(6756l, v[5].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1107l, v[5].blobSize);
    CPPUNIT_ASSERT_EQUAL(7879l, v[6].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1107l, v[6].blobSize);
    CPPUNIT_ASSERT_EQUAL(9002l, v[7].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1108l, v[7].blobSize);
    CPPUNIT_ASSERT_EQUAL(10126l, v[8].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1108l, v[8].blobSize);
    CPPUNIT_ASSERT_EQUAL(11250l, v[9].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1111l, v[9].blobSize);
    CPPUNIT_ASSERT_EQUAL(12377l, v[10].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1111l, v[10].blobSize);
    CPPUNIT_ASSERT_EQUAL(13504l, v[11].blobOffset);
    CPPUNIT_ASSERT_EQUAL(1111l, v[11].blobSize);

    for (size_t i = 0; i < v.size(); i++)
    {
      uut.parseBlob(v[i], &input, map);
    }

    // sanity check that it actually read the data.
    CPPUNIT_ASSERT_EQUAL(36, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(4, (int)map->getWays().size());
  }

  void runReadIncrementalTest()
  {

    // input data generated by OsmPbfWriterTest
    const unsigned char data[] = {
        0,   0,   0,  13,  10,   9,  79,  83,  77,  72, 101,  97, 100, 101, 114,  24,  76,  16,
       64,  26,  72, 120, 156,  83, 226, 243,  47, 206,  13,  78, 206,  72, 205,  77, 212,  13,
       51, 208,  51,  83, 226, 114,  73, 205,  43,  78, 245, 203,  79,  73,  45, 110,  98,  84,
      244, 200, 207,  47,  73, 205,  75, 204, 203, 171,  84,  48, 208,  51, 212,  51, 210,  53,
      214,  77,  55,  73, 177, 180,  48,  79,  76, 210,  77, 201,  44,  42, 169,   4,   0, 131,
       80,  20,   2,   0,   0,   0,  12,  10,   7,  79,  83,  77,  68,  97, 116,  97,  24, 195,
        1,  16, 189,   1,  26, 189,   1, 120, 156, 227, 242, 227,  98, 224,  98, 201, 203,  47,
       73, 229, 226,  40,  73,  45,  46,  81,  40,  73,  76, 231,  98, 205,  72, 205, 201, 201,
      231,  98,  45, 207,  47, 202,  73, 225,  98, 207, 200,  76, 207,  40,  79, 172, 228,  98,
       41, 202,  79,  76, 225, 226,  72,  76,  78,  46,  45,  74,  76, 174, 228,  98,  54, 212,
       51, 231, 226, 206, 200, 207,  47, 177,  42,  46,  73,  44,  41,  45, 230,  98,  52,  20,
       82,  19,  82, 225,  98, 158, 192, 200, 228, 196, 185, 227, 231,  47, 217,  19, 147,  38,
       53,  48, 121, 113,  53,  28,  57,  58, 129, 249, 255, 238, 231,  23,  25, 131, 216,  24,
      153, 152,  89,  24,  24, 132, 156, 165, 196,  57, 180, 132,  88,  25, 153,  89, 217,  57,
      165,  88, 153,  88, 216,  56, 184, 156, 128,  20,  11,  11, 135, 148,   6, 135, 182,  16,
       19,  80, 148,   9,  40, 196,  52, 137, 209,  75, 130, 163, 161, 227, 222,  62,  70,   1,
       48,  41, 209, 208, 211,  59, 143,  73,   1,  76,   2,   0,  66, 177,  50,  48};
    size_t dataSize = 304;

    string s;
    s.resize(dataSize);
    memcpy((char*)s.data(), data, dataSize);
    stringstream ss(s, stringstream::in);

    OsmMapPtr map(new OsmMap());

    OsmPbfReader reader(true);
    reader.setUseFileStatus(true);
    reader.parse(&ss, map);

    string expected("highway = road\nnote = test tag\nhello = world\n");
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(expected, map->getWay(42)->getTags().toString().toStdString());
    CPPUNIT_ASSERT_EQUAL((size_t)5, map->getWay(42)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)1, map->getWay(43)->getNodeCount());
    long nodeIds[] = {1, 3, 5, 7, 11};
    for (int i = 0; i < 5; i++)
    {
      CPPUNIT_ASSERT_EQUAL(nodeIds[i], map->getWay(42)->getNodeId(i));
    }
  }

  void runReadNodeTest()
  {
    // input data generated by OsmPbfWriterTest
    const unsigned char data[] = {
        0,   0,   0,  60,  10,  32,  10,   0,  10,   4, 110, 111, 116, 101,  10,   8, 116, 101,
      115, 116,  32, 116,  97, 103,  10,   5, 104, 101, 108, 108, 111,  10,   5, 119, 111, 114,
      108, 100,  18,  24,  10,  22,   8, 144,   1,  18,   2,   1,   3,  26,   2,   2,   4,  64,
      184, 249, 250,  29,  72, 128, 196, 197, 144,   3};
    size_t dataSize = 64;

    string s;
    s.resize(dataSize);
    memcpy((char*)s.data(), data, dataSize);
    stringstream ss(s, stringstream::in);

    OsmMapPtr map(new OsmMap());

    OsmPbfReader reader(true);
    reader.setPermissive(true);
    reader.parseElements(&ss, map);

    string expected("note = test tag\nhello = world\n");
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(expected, map->getNode(72)->getTags().toString().toStdString());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(42.0, map->getNode(72)->getX(), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.14159, map->getNode(72)->getY(), 0.0001);
  }

  void runReadRelationTest()
  {
    // input data generated by OsmPbfWriterTest
    const unsigned char data[] = {
        0,   0,   0, 139,  10,  96,  10,   0,  10,   1, 115,  10,   1, 116,  10,   1, 117,  10,
        1, 102,  10,   4, 110, 111, 116, 101,  10,   8, 116, 101, 115, 116,  32, 116,  97, 103,
       10,   5, 104, 101, 108, 108, 111,  10,   5, 119, 111, 114, 108, 100,  10,   7, 104, 105,
      103, 104, 119,  97, 121,  10,   4, 114, 111,  97, 100,  10,  14, 101, 114, 114, 111, 114,
       58,  99, 105, 114,  99, 117, 108,  97, 114,  10,   3,  49,  46,  55,  10,  11, 104, 111,
      111, 116,  58, 115, 116,  97, 116, 117, 115,  10,   1,  49,  18,  39,  34,  37,   8,  42,
       18,   5,   5,   7,   9,  11,  13,  26,   5,   6,   8,  10,  12,  14,  66,   5,   1,   2,
        3,   4,   4,  74,   5,   2,   2,   2,   3,   0,  82,   5,   0,   0,   0,   1,   2};
    size_t dataSize = 143;

    string s;
    s.resize(dataSize);
    memcpy((char*)s.data(), data, dataSize);
    stringstream ss(s, stringstream::in);

    OsmMapPtr map(new OsmMap());

    OsmPbfReader reader(true);
    reader.setPermissive(true);
    reader.parseElements(&ss, map);

    OsmJsonWriter writer;
    writer.setIncludeCompatibilityTags(false);
    const QString actual = writer.toString(map);
    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                    "{\"type\":\"relation\",\"id\":42,\"members\":[\n"
                    "{\"type\":\"node\",\"ref\":1,\"role\":\"s\"},\n"
                    "{\"type\":\"node\",\"ref\":2,\"role\":\"t\"},\n"
                    "{\"type\":\"node\",\"ref\":3,\"role\":\"u\"},\n"
                    "{\"type\":\"way\",\"ref\":1,\"role\":\"f\"},\n"
                    "{\"type\":\"relation\",\"ref\":1,\"role\":\"f\"}],\"tags\":{\"highway\":\"road\",\"note\":\"test tag\",\"hello\":\"world\",\"" + MetadataTags::ErrorCircular() + "\":\"1.7\"}}]\n"
                    "}\n",
                    actual);
    CPPUNIT_ASSERT(OsmJsonReader().isValidJson(actual));
  }

  void runReadWayTest()
  {
    // input data generated by OsmPbfWriterTest
    const unsigned char data[] = {
        0,   0,   0,  89,  10,  62,  10,   0,  10,   4, 110, 111, 116, 101,  10,   8, 116, 101,
      115, 116,  32, 116,  97, 103,  10,   5, 104, 101, 108, 108, 111,  10,   5, 119, 111, 114,
      108, 100,  10,   7, 104, 105, 103, 104, 119,  97, 121,  10,   4, 114, 111,  97, 100,  10,
        8,  97,  99,  99, 117, 114,  97,  99, 121,  10,   3,  49,  46,  55,  18,  23,  26,  21,
        8,  42,  18,   4,   1,   3,   5,   7,  26,   4,   2,   4,   6,   8,  66,   5,   2,   4,
        4,   4,   8};
    size_t dataSize = 93;

    string s;
    s.resize(dataSize);
    memcpy((char*)s.data(), data, dataSize);
    stringstream ss(s, stringstream::in);

    OsmMapPtr map(new OsmMap());

    OsmPbfReader reader(true);
    reader.setPermissive(true);
    reader.parseElements(&ss, map);

    string expected("highway = road\nnote = test tag\nhello = world\n");
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(expected, map->getWay(42)->getTags().toString().toStdString());
    CPPUNIT_ASSERT_EQUAL((size_t)5, map->getWay(42)->getNodeCount());
    long nodeIds[] = {1, 3, 5, 7, 11};
    for (int i = 0; i < 5; i++)
    {
      CPPUNIT_ASSERT_EQUAL(nodeIds[i], map->getWay(42)->getNodeId(i));
    }
  }

  void runToyTest()
  {
    OsmPbfReader uut(false);
    fstream input("test-files/ToyTestA.osm.pbf", ios::in | ios::binary);
    OsmMapPtr map(new OsmMap());
    uut.parse(&input, map);

    OsmXmlWriter writer;
    writer.setIncludeHootInfo(false);
    writer.write(map, _outputPath + "OsmPbfReaderTest_1.osm");

    HOOT_FILE_EQUALS(_inputPath + "OsmPbfReaderTest.osm",
                     _outputPath + "OsmPbfReaderTest_1.osm");
  }

  void runToyRelationTest()
  {
    OsmPbfReader uut(false);
    fstream input(_inputPath.toStdString() + "OsmPbfRelationTest.osm.pbf", ios::in | ios::binary);
    OsmMapPtr map(new OsmMap());
    uut.parse(&input, map);

    const QString testFileName = "runToyRelationTest.json";
    OsmJsonWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  void runIsSupportedUrlExistsTest()
  {
    OsmPbfReader reader(false);
    CPPUNIT_ASSERT(reader.isSupported("test-files/ToyTestA.osm.pbf"));
  }

  void runIsSupportedUrlDoesntExistTest()
  {
    OsmPbfReader reader(false);
    CPPUNIT_ASSERT(!reader.isSupported("test-files/fileDoesntExist.osm.pbf"));
  }

  void runIsSupportedUrlBadExtensionTest()
  {
    OsmPbfReader reader(false);
    CPPUNIT_ASSERT(!reader.isSupported("test-files/fileDoesntExist.osm"));
  }

  //in the future might try to support dir urls with this interface, but for now we don't
  void runIsSupportedUrlIsDirTest()
  {
    OsmPbfReader reader(false);
    CPPUNIT_ASSERT(!reader.isSupported("test-files"));
  }

  void runOpenUrlExistsTest()
  {
    OsmPbfReader reader(false);
    //shouldn't throw
    reader.open("test-files/ToyTestA.osm.pbf");
    reader.close();
  }

  void runOpenUrlDoesntExistTest()
  {
    OsmPbfReader reader(false);
    QString exceptionMsg("");
    try
    {
      reader.open("test-files/fileDoesntExist.osm.pbf");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(QString("Error opening test-files/fileDoesntExist.osm.pbf for reading."),
                    exceptionMsg);
  }

  void runReadMapTest()
  {
    OsmPbfReader reader(false);
    OsmMapPtr map(new OsmMap());
    reader.open("test-files/ToyTestA.osm.pbf");
    reader.read(map);
    reader.close();

    OsmXmlWriter writer;
    writer.setIncludeHootInfo(false);
    writer.write(map, _outputPath + "OsmPbfReaderTest_2.osm");

    HOOT_FILE_EQUALS( _inputPath + "OsmPbfReaderTest.osm",
                     _outputPath + "OsmPbfReaderTest_2.osm");
  }

  void runFactoryReadMapTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm.pbf", false, Status::Unknown1);

    OsmXmlWriter writer;
    writer.setIncludeHootInfo(false);
    writer.write(map, _outputPath + "OsmPbfReaderTest_3.osm");

    HOOT_FILE_EQUALS( _inputPath + "OsmPbfReaderTest.osm",
                     _outputPath + "OsmPbfReaderTest_3.osm");
  }

  void runReadMapPartialTest()
  {
    OsmPbfReader reader(false);
    const int chunkSize = 10;
    reader.setMaxElementsPerMap(chunkSize);
    reader.open("test-files/ToyTestA.osm.pbf");
    reader.initializePartial();

    OsmXmlWriter writer;
    writer.setIncludeHootInfo(false);

    //Suppress the warning from the OsmXmlReader about missing nodes for ways by temporarily changing
    //the log level.  We expect the nodes to be missing since we're doing partial map reads and
    //don't need to see the messages.
    Log::WarningLevel logLevel = Log::getInstance().getLevel();
    if (Log::getInstance().getLevel() >= Log::Info)
      Log::getInstance().setLevel(Log::Error);

    int ctr = 0;
    while (reader.hasMoreElements())
    {
      OsmMapPtr map(new OsmMap());
      reader.readPartial(map);
      CPPUNIT_ASSERT_EQUAL(
        chunkSize,
        (int)(map->getNodes().size() + map->getWays().size() + map->getRelations().size()));

      QString outputFile(
        _outputPath + "OsmPbfPartialReaderTest" + QString::number(ctr + 1) + ".osm");
      writer.write(map, outputFile);
      HOOT_FILE_EQUALS(_inputPath + "OsmPbfPartialReaderTest" + QString::number(ctr + 1) + ".osm",
        outputFile);

      ctr++;
      CPPUNIT_ASSERT(ctr < 5);  //to prevent an infinite loop if hasMoreElements fails
    }
    Log::getInstance().setLevel(logLevel);
    reader.finalizePartial();

    CPPUNIT_ASSERT_EQUAL(4, ctr);
  }

  void runReadMapPartialMultipleBlobsTest()
  {
    OsmPbfReader reader(false);
    const int chunkSize = 40;
    reader.setMaxElementsPerMap(chunkSize);
    reader.open("test-files/ToyTestCombined.osm.pbf");
    reader.initializePartial();

    OsmXmlWriter writer;
    writer.setIncludeHootInfo(false);

    //Suppress the warning from the OsmXmlReader about missing nodes for ways by temporarily changing
    //the log level.  We expect the nodes to be missing since we're doing partial map reads and
    //don't need to see the messages.
    Log::WarningLevel logLevel = Log::getInstance().getLevel();
    if (Log::getInstance().getLevel() >= Log::Info)
      Log::getInstance().setLevel(Log::Error);

    int ctr = 0;
    while (reader.hasMoreElements())
    {
      OsmMapPtr map(new OsmMap());
      reader.readPartial(map);

      //some of these before the last one don't read out the full buffer size..not sure why
      CPPUNIT_ASSERT(
        (int)(map->getNodes().size() + map->getWays().size() + map->getRelations().size()) <=
        chunkSize);

      QString outputFile(
        _outputPath + "OsmPbfPartialReaderMultipleBlobsTest" + QString::number(ctr + 1) + ".osm");
      writer.write(map, outputFile);
      HOOT_FILE_EQUALS(
        _inputPath + "OsmPbfPartialReaderMultipleBlobsTest" + QString::number(ctr + 1) + ".osm",
        outputFile);

      ctr++;
      CPPUNIT_ASSERT(ctr < 5);  //to prevent an infinite loop if hasMoreElements fails
    }
    Log::getInstance().setLevel(logLevel);
    reader.finalizePartial();

    CPPUNIT_ASSERT_EQUAL(4, ctr);
  }

  void runHasMoreElementsTest()
  {
    OsmPbfReader reader1;

    // If we haven't opened a file, it best not be ready to read
    CPPUNIT_ASSERT_EQUAL(reader1.hasMoreElements(), false);

    // Try to open invalid file
    OsmPbfReader reader2(QString("test-files/totalgarbage.osm.pbf"));
    CPPUNIT_ASSERT_EQUAL(reader2.hasMoreElements(), false);

    // Open valid file
    OsmPbfReader reader3(QString("test-files/ToyTestA.osm.pbf"));
    CPPUNIT_ASSERT_EQUAL(reader3.hasMoreElements(), true);

    // Close file and check again
    reader3.close();
    CPPUNIT_ASSERT_EQUAL(reader3.hasMoreElements(), false);
  }

  void runReadNextElementTest()
  {
    OsmPbfReader reader(QString("test-files/ToyTestA.osm.pbf"));

    // Iterate through all items
    int numberOfElements(0);
    while ( reader.hasMoreElements() == true )
    {
      ElementPtr tempElement = reader.readNextElement();
      numberOfElements++;
      LOG_VART(tempElement->toString())
    }

    CPPUNIT_ASSERT_EQUAL(40, numberOfElements);
  }

  void runReadSortTypeTest()
  {
    //This pbf file contains Sort.Type_then_ID in the header. Test to read it.
    OsmMapPtr map(new OsmMap());
    OsmPbfReader reader(true);
    reader.open("test-files/OsmPbfPartialReaderTest4_with_sorttype.osm.pbf");
    reader.read(map);

    CPPUNIT_ASSERT_EQUAL(true, reader.getSortedTypeThenId());
    CPPUNIT_ASSERT_EQUAL(6, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(3, (int)map->getWay(-1)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(3, (int)map->getWay(-2)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWay(-3)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWay(-4)->getNodeCount());

    //The test is for #161 - OsmPbfReader should be more permissive when the file is unsorted
    //This file doesn't have Sort.Type_then_ID in the header. Before changes, when permissive
    //set to false, the nodes count with ways are all zeros. Now the ways contain valid nodes.
    OsmMapPtr map1(new OsmMap());
    OsmPbfReader reader1(true);
    reader1.open("test-files/OsmPbfPartialReaderTest4_without_sorttype.osm.pbf");
    reader1.setPermissive(false);

    //Suppress the warning from the OsmXmlReader about missing nodes for ways by temporarily changing
    //the log level.  We expect the nodes to be missing since we're doing partial map reads and
    //don't need to see the messages.
    Log::WarningLevel logLevel = Log::getInstance().getLevel();
    if (Log::getInstance().getLevel() >= Log::Info)
      Log::getInstance().setLevel(Log::Error);
    reader1.read(map1);

    CPPUNIT_ASSERT_EQUAL(false, reader1.getSortedTypeThenId());
    CPPUNIT_ASSERT_EQUAL(6, (int)map1->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(3, (int)map1->getWay(-1)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(3, (int)map1->getWay(-2)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(1, (int)map1->getWay(-3)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(1, (int)map1->getWay(-4)->getNodeCount());


    //test the pbf file that the sorted flag isn't set and values are out of order
    OsmMapPtr map2(new OsmMap());
    OsmPbfReader reader2(true);
    reader2.open("test-files/OsmPbfTest_withoursoretype_unsorted.osm.pbf");
    reader2.setPermissive(false);

    reader2.read(map2);
    Log::getInstance().setLevel(logLevel);

    CPPUNIT_ASSERT_EQUAL(false, reader2.getSortedTypeThenId());
    CPPUNIT_ASSERT_EQUAL(6, (int)map2->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(3, (int)map2->getWay(-1)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(3, (int)map2->getWay(-2)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(1, (int)map2->getWay(-3)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(1, (int)map2->getWay(-4)->getNodeCount());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmPbfReaderTest, "quick");

}
