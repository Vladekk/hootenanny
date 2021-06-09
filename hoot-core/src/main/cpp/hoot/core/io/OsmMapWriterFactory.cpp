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
#include "OsmMapWriterFactory.h"

// hoot
#include <hoot/core/conflate/network/DebugNetworkMapCreator.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/geometry/GeometryToElementConverter.h>
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/io/ElementOutputStream.h>
#include <hoot/core/schema/SchemaUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

unsigned int OsmMapWriterFactory::_debugMapCount = 1;

std::shared_ptr<OsmMapWriter> OsmMapWriterFactory::createWriter(const QString& url)
{
  LOG_VART(url);

  QString writerOverride = ConfigOptions().getMapFactoryWriter();
  LOG_VART(writerOverride);

  std::shared_ptr<OsmMapWriter> writer;
  if (writerOverride != "" && url != ConfigOptions().getDebugMapsFilename())
  {
    writer.reset(Factory::getInstance().constructObject<OsmMapWriter>(writerOverride));
  }

  vector<QString> names = Factory::getInstance().getObjectNamesByBase(OsmMapWriter::className());
  for (size_t i = 0; i < names.size() && !writer; ++i)
  {
    LOG_VART(names[i]);
    writer.reset(Factory::getInstance().constructObject<OsmMapWriter>(names[i]));

    // We may be able to make this a little more generic by referencing an interface instead.
    // Currently, OgrWriter is the only writer that runs a schema translation inline.
    std::shared_ptr<OgrWriter> ogrWriter = std::dynamic_pointer_cast<OgrWriter>(writer);
    if (ogrWriter)
    {
      const QString translationScript = ConfigOptions().getSchemaTranslationScript();
      if (!translationScript.isEmpty())
      {
        SchemaUtils::validateTranslationUrl(translationScript);
        ogrWriter->setSchemaTranslationScript(translationScript);
      }
    }

    if (writer->isSupported(url))
    {
      LOG_DEBUG("Using output writer: " << names[i]);
    }
    else
    {
      writer.reset();
    }
  }

  if (!writer)
  {
    throw HootException("A valid writer could not be found for the URL: " + url);
  }

  return writer;
}

QString OsmMapWriterFactory::getWriterName(const QString& url)
{
  LOG_VARD(url);
  vector<QString> names = Factory::getInstance().getObjectNamesByBase(OsmMapWriter::className());
  LOG_VARD(names.size());
  std::shared_ptr<OsmMapWriter> writer;
  for (size_t i = 0; i < names.size(); i++)
  {
    const QString name = names[i];
    LOG_VART(name);
    writer.reset(Factory::getInstance().constructObject<OsmMapWriter>(name));
    if (writer->isSupported(url))
    {
      return name;
    }
  }
  return "";
}

bool OsmMapWriterFactory::hasElementOutputStream(const QString& url)
{
  bool result = false;
  std::shared_ptr<OsmMapWriter> writer = createWriter(url);
  std::shared_ptr<ElementOutputStream> streamWriter =
    std::dynamic_pointer_cast<ElementOutputStream>(writer);
  if (streamWriter)
  {
    result = true;
  }
  return result;
}

void OsmMapWriterFactory::write(
  const std::shared_ptr<OsmMap>& map, const QString& url, const bool silent, const bool is_debug)
{
  bool skipEmptyMap = map->isEmpty() && ConfigOptions().getMapWriterSkipEmpty();

  if (!silent)
  {
    LOG_INFO(
      (skipEmptyMap ? "Map is empty. Not writing to " : "Writing map to ") << url << "...");
  }

  if (!skipEmptyMap)
  {
    QElapsedTimer timer;
    timer.start();

    MapProjector::projectToWgs84(map);

    // TODO: For debug maps, we don't want to log the progress of the writing to cut down on log
    // statements.
    std::shared_ptr<OsmMapWriter> writer = createWriter(url);
    writer->setIsDebugMap(is_debug);
    writer->open(url);
    // We could pass a progress in here to get more granular write status feedback.
    writer->write(map);
    LOG_INFO(
      "Wrote " << StringUtils::formatLargeNumber(map->getElementCount()) <<
      " elements to output in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
  }
}

void OsmMapWriterFactory::writeDebugMap(
  const ConstOsmMapPtr& map, const QString& callingClass, const QString& title,
  NetworkMatcherPtr matcher)
{
  if (ConfigOptions().getDebugMapsWrite())
  {
    if (callingClass.trimmed().isEmpty())
    {
      throw IllegalArgumentException("Empty calling class.");
    }
    else if (!callingClass.startsWith("hoot::"))
    {
      throw IllegalArgumentException("Invalid calling class: " + callingClass);
    }

    LOG_VARD(ConfigOptions().getDebugMapsWrite());
    LOG_VARD(callingClass);

    QStringList classFilter = ConfigOptions().getDebugMapsClassFilter().split(";");
    StringUtils::removeEmptyStrings(classFilter);
    LOG_VARD(classFilter);
    QString callingClassNoNamespace = callingClass;
    callingClassNoNamespace = callingClassNoNamespace.replace("hoot::", "");
    if (classFilter.isEmpty() || classFilter.contains(callingClassNoNamespace))
    {
      QString debugMapFileName = ConfigOptions().getDebugMapsFilename();
      if (!debugMapFileName.toLower().endsWith(".osm"))
      {
        throw IllegalArgumentException("Debug maps must be written to an .osm file.");
      }

      LOG_VART(StringUtils::formatLargeNumber(map->getElementCount()));
      LOG_VART(StringUtils::formatLargeNumber(map->getNodeCount()));
      LOG_VART(StringUtils::formatLargeNumber(map->getWayCount()));
      LOG_VART(StringUtils::formatLargeNumber(map->getRelationCount()));

      const QString fileNumberStr =
        StringUtils::padFrontOfNumberStringWithZeroes(_debugMapCount, 4);
      if (!title.isEmpty())
      {
        debugMapFileName =
          debugMapFileName.replace(
            ".osm", "-" + fileNumberStr + "-" + callingClassNoNamespace + "-" + title + ".osm");
      }
      else
      {
        debugMapFileName =
          debugMapFileName.replace(
            ".osm", "-" + fileNumberStr + "-" + callingClassNoNamespace + ".osm");
      }
      LOG_INFO("Writing debug output to: ..." << FileUtils::toLogFormat(debugMapFileName, 30));
      OsmMapPtr copy(new OsmMap(map));

      if (matcher)
      {
        DebugNetworkMapCreator()
          .addDebugElements(copy, matcher->getAllEdgeScores(), matcher->getAllVertexScores());
      }

      MapProjector::projectToWgs84(copy);
      if (ConfigOptions().getDebugMapsRemoveMissingElements())
      {
        // Don't remove elements recursively here. You can end up with a map unreadable in JOSM if
        // you don't remove missing elements here. However, in some cases (like debugging cut and
        // replace), you want to see them in the raw output to know things are working the way they
        // should be.
        RemoveMissingElementsVisitor missingElementsRemover;
        copy->visitRw(missingElementsRemover);
      }
      write(copy, debugMapFileName, true, true);
      _debugMapCount++;
    }
  }
}

void OsmMapWriterFactory::writeDebugMap(
  const std::shared_ptr<geos::geom::Geometry>& geometry,
  std::shared_ptr<OGRSpatialReference> spatRef, const QString& callingClass, const QString& title,
  NetworkMatcherPtr matcher)
{
  OsmMapPtr map(new OsmMap(spatRef));
  // add the resulting alpha shape for debugging.
  GeometryToElementConverter(map).convertGeometryToElement(geometry.get(), Status::Invalid, -1);

  writeDebugMap(map, callingClass, title, matcher);
}

}
