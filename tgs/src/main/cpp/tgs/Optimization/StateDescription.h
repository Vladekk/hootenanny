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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef STATEDESCRIPTION_H
#define STATEDESCRIPTION_H

// Qt
#include <QList>

// Tgs
#include <tgs/Optimization/VariableDescription.h>

namespace Tgs
{

class StateDescription
{
public:

  StateDescription() = default;

  void addVariable(ConstVariableDescriptionPtr v) { _variables.append(v); }
  void addVariable(VariableDescription* v) { _variables.append(VariableDescriptionPtr(v)); }

  QList<ConstVariableDescriptionPtr> getVariables() const { return _variables; }

private:

  QList<ConstVariableDescriptionPtr> _variables;
};

using StateDescriptionPtr = std::shared_ptr<StateDescription>;
using ConstStateDescriptionPtr = std::shared_ptr<const StateDescription>;

}

#endif // STATEDESCRIPTION_H
