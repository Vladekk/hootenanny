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

#ifndef BOUNDED_COMMAND_H
#define BOUNDED_COMMAND_H

// geos
#include <geos/geom/Envelope.h>

// Hoot
#include <hoot/core/cmd/BaseCommand.h>

namespace hoot
{

/**
 * @brief The BoundedCommand class is a command that operates within a bounds.
 */
class BoundedCommand : public BaseCommand
{
public:

  BoundedCommand();
  virtual ~BoundedCommand() = default;

  int runSimple(QStringList& args) override;

protected:

  virtual void _writeBoundsFile();

private:

  // can be useful for debugging
  bool _writeInternalEnvelope;
};

}

#endif // BOUNDED_COMMAND_H
