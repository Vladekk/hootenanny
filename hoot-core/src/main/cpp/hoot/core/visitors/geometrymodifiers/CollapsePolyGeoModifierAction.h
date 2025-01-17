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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef COLLAPSEPOLYGEOMODIFIERACTION_H
#define COLLAPSEPOLYGEOMODIFIERACTION_H

#include "GeometryModifierAction.h"

namespace hoot
{

/*
 * Collapses a filtered polygon feature to a single point node at the location
 * of the polygons centroid.
 * The polygon is collapsed only if its area is smaller than max_area_in_m or its
 * maximum length is smaller than max_length_in_m.
 * If either parameter is set to 0, it is ignored.
 */
class CollapsePolyGeoModifierAction : public GeometryModifierAction
{
public:
  static const QString MAX_AREA_PARAM;
  static const QString MAX_LENGTH_PARAM;

  static QString className() { return "hoot::CollapsePolyGeoModifierAction"; }

  CollapsePolyGeoModifierAction() = default;
  ~CollapsePolyGeoModifierAction() = default;

  QString getCommandName() const override { return "collapse_poly"; }
  QList<QString> getParameterNames() const override
  { return QList<QString> { MAX_AREA_PARAM, MAX_LENGTH_PARAM }; }

  void parseArguments(const QHash<QString, QString>& arguments) override;
  bool processElement(const ElementPtr& pElement, OsmMap* pMap) override;

private:
  const double DEFAULT_AREA = 15000;
  const double DEFAULT_LENGTH = 0;

  double _area = DEFAULT_AREA;
  double _length = DEFAULT_LENGTH;
};

}

#endif // COLLAPSEPOLYGEOMODIFIERACTION_H

