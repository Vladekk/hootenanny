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
#ifndef OSMCHANGEWRITER_H
#define OSMCHANGEWRITER_H

// hoot
#include <hoot/core/algorithms/changeset/ChangesetProvider.h>

namespace hoot
{

/**
 * Interface for writing OSM changes
 *
 * There may be some redundancy with this and OsmChangesetFileWriter.
 */
class OsmChangeWriter
{

public:

  static QString className() { return "hoot::OsmChangeWriter"; }

  OsmChangeWriter() = default;
  virtual ~OsmChangeWriter() = default;

  /**
   * Writes an OSM change.
   *
   * @param change an OSM change
   */
  virtual void writeChange(const Change& change) = 0;

  /**
   * Returns true if the output URL is supported.
   */
  virtual bool isSupported(const QString& url) = 0;

  /**
   * Opens the specified URL for writing.
   */
  virtual void open(const QString& url) = 0;

  /**
   * Closes the output file
   */
  virtual void close() = 0;

  // this is kind of kludgy in that its only required for the SparkChangesetReader; think of
  // a way to get rid of this
  virtual void setElementPayloadFormat(const QString& format) = 0;

};

}

#endif // OSMCHANGEWRITER_H
