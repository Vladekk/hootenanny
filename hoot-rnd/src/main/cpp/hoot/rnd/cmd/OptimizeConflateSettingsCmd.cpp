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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/rnd/conflate/opt/ConflateSettingsOptimizer.h>
#include <hoot/rnd/conflate/network/opt/CaseTestFitnessFunction.h>
#include <hoot/rnd/conflate/network/opt/ReleaseTestFitnessFunction.h>
#include <hoot/rnd/conflate/network/opt/PertyTestFitnessFunction.h>

using namespace std;

namespace hoot
{

class OptimizeConflateSettingsCmd : public BaseCommand
{
public:

  static string className() { return "hoot::OptimizeConflateSettingsCmd"; }

  OptimizeConflateSettingsCmd() { }

  virtual QString getName() const { return "optimize-conflate-settings"; }

  /*
   * Most of the time you want to run this at the error log level to reduce log clutter.
   * Also when running them, temporarily uncomment marked cout lines in SimulatedAnnealing::iterate
   * for better logging feedback.
   *
   * TODO: come up with a better way to control logging inside SimulatedAnnealing than uncommenting
   * cout lines
   */
  virtual int runSimple(QStringList args)
  {
    if (args.size() < 5 || args.size() > 6)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes five or six parameters.").arg(getName()));
    }

    const QString testsDir = args[0];
    const QString testType = args[1].toLower();
    if (testType != "case" && testType != "release" && testType != "perty")
    {
      throw HootException("Invalid test type specified: " + testType);
    }
    const QString testSettingsPath = args[2];
    bool parsedIterations = false;
    const int numIterations = args[3].toInt(&parsedIterations);
    if (!parsedIterations)
    {
      throw HootException("Invalid number of test iterations: " + args[3]);
    }
    const QString summaryFilePath = args[4];
    QString configFilePath = "";
    if (args.size() == 6)
    {
      configFilePath = args[5];
    }

    boost::shared_ptr<AbstractTestFitnessFunction> fitnessFunction;
    if (testType == "case")
    {
      fitnessFunction.reset(new CaseTestFitnessFunction(testsDir, configFilePath));
    }
    else if (testType == "release")
    {
      fitnessFunction.reset(new ReleaseTestFitnessFunction(testsDir, configFilePath));
    }
    else
    {
      fitnessFunction.reset(new PertyTestFitnessFunction(testsDir, configFilePath));
    }

    ConflateSettingsOptimizer().runOptimization(
      fitnessFunction, numIterations, testSettingsPath, summaryFilePath);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, OptimizeConflateSettingsCmd)

}

