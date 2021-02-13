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

#include "ElementComparison.h"

// Hoot


namespace hoot
{

ElementComparison::ElementComparison(ElementPtr element, const OsmMap& sourceMap,
                                     const bool ignoreElementId) :
_element(element)
{
  _elementComparer.setIgnoreElementId(ignoreElementId);
  OsmMap* map = const_cast<OsmMap*>(&sourceMap);
  _elementComparer.setOsmMap(map);
}

bool ElementComparison::operator==(const ElementComparison& elementComp) const
{
  const bool equals = _elementComparer.isSame(this->_element, elementComp.getElement());
  if (equals)
  {
    LOG_TRACE(
      "Elements equal: " << this->_element->getElementId() << " and " <<
      elementComp.getElement()->getElementId())
  }
  return equals;
}

QString ElementComparison::toString() const
{
  return _element->getElementId().toString();
}

}
