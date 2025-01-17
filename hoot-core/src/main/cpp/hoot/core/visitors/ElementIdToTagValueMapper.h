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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENT_ID_TO_TAG_VALUE_MAPPER_H
#define ELEMENT_ID_TO_TAG_VALUE_MAPPER_H

// hoot
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/**
 * Creates a mapping between an element ID and the value of the specified tag, if it has the tag
 */
class ElementIdToTagValueMapper : public ConstElementVisitor
{

public:

  static QString className() { return "hoot::ElementIdToTagValueMapper"; }

  ElementIdToTagValueMapper() = default;
  ~ElementIdToTagValueMapper() = default;

  void visit(const ConstElementPtr& e) override;

  QString getInitStatusMessage() const override { return "Mapping tag values..."; }
  QString getCompletedStatusMessage() const override
  { return "Mapped " + StringUtils::formatLargeNumber(_numAffected) + " tag values."; }

  QMap<ElementId, QString> getIdToTagValueMappings() const { return _idToTagValueMappings; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Maps element IDs to tag values for a given tag key"; }

  void setTagKey(const QString& key) { _tagKey = key; }

private:

  QString _tagKey;
  QMap<ElementId, QString> _idToTagValueMappings;
};

}

#endif // ELEMENT_ID_TO_TAG_VALUE_MAPPER_H
