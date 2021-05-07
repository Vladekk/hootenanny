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

// GEOS
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/CookieCutter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{
using namespace std;

class CutCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::CutCmd"; }

  CutCmd() = default;

  QString getName() const override { return "cut"; }
  QString getDescription() const override { return "Cuts out a portion from a map"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.size() < 3 || args.size() > 5)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three to five parameters.").arg(getName()));
    }

    int i = 0;
    QString cutterShapePath = args[i++];
    QString doughPath = args[i++];
    QString outputPath = args[i++];
    bool ok = false;
    double buffer = args.size() > i ? args[i].toDouble(&ok) : 0.0;
    if (ok)
    {
      i++;
    }
    else
    {
      buffer = 0.0;
    }
    bool crop = false;
    if (args.size() > i)
    {
      if (args[i++] == "--crop")
      {
        crop = true;
      }
      else
      {
        throw HootException(
          QString("Expected --crop as the last argument, but got %1.").
            arg(args[i - 1]));
      }
    }

    LOG_STATUS(
      "Cutting ..." << FileUtils::toLogFormat(cutterShapePath, 25) << " out of ..." <<
      FileUtils::toLogFormat(doughPath, 25) << " and writing output to ..." <<
      FileUtils::toLogFormat(outputPath, 25) << "...");

    // load up the shape being cut out
    OsmMapPtr cutterShapeMap(new OsmMap());
    IoUtils::loadMap(cutterShapeMap, cutterShapePath, true, Status::Unknown1);

    // load up the dough
    OsmMapPtr doughMap(new OsmMap());
    IoUtils::loadMap(doughMap, doughPath, true, Status::Unknown1);

    // cut the cutter shape out of the dough
    CookieCutter(crop, buffer).cut(cutterShapeMap, doughMap);
    OsmMapPtr result = doughMap;

    MapProjector::projectToWgs84(result);
    IoUtils::saveMap(result, outputPath);

    LOG_STATUS(
      "Map cookie cut in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, CutCmd)

}
