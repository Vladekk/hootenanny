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

#ifndef OSM_API_WRITER_TEST_SERVER_H
#define OSM_API_WRITER_TEST_SERVER_H

#include <hoot/core/util/HttpTestServer.h>

namespace hoot
{

class CapabilitiesTestServer : public HttpTestServer
{
public:
  /** Constructor */
  CapabilitiesTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that responds once to the OSM API Capabilities request */
  bool respond(HttpConnectionPtr& connection) override;
};

class PermissionsTestServer : public HttpTestServer
{
public:
  /** Constructor */
  PermissionsTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that responds once to the OSM API Permissions request */
  bool respond(HttpConnectionPtr &connection) override;
};

class RetryConflictsTestServer : public HttpTestServer
{
public:
  /** Constructor */
  RetryConflictsTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that responds to a series of OSM API requests
   *  to simulate a 405 Method Not Allowed error to test error resolution
   *  Requests, in order:
   *   - Capabilities
   *   - Permissions
   *   - Changeset Create
   *   - Changeset Upload - responding with an HTTP 405 error for the test
   *   - Changeset Close
   */
  bool respond(HttpConnectionPtr& connection) override;
};

class RetryVersionTestServer : public HttpTestServer
{
public:
  /** Constructor */
  RetryVersionTestServer(int port) : HttpTestServer(port), _has_error(false) { }

protected:
  /** respond() function that responds to a series of OSM API requests
   *  to simulate a mismatched element version
   *  Requests, in order:
   *  - Capabilities
   *  - Permissions
   *  - Changeset Create
   *  - Changeset 1 Upload - respond with an HTTP 404 error for the test
   *  - Element get
   *  - Changeset 1 Upload - respond with updated version
   *  - Changeset Close
   */
  bool respond(HttpConnectionPtr &connection) override;

private:
  /** Flag set to false until the first changeset has failed once */
  bool _has_error;
};

class ChangesetOutputTestServer : public HttpTestServer
{
public:
  /** Constructor */
  ChangesetOutputTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that responds to a series of OSM API requests
   *  to simulate a changeset upload.
   *  Requests, in order:
   *   - Capabilities
   *   - Permissions
   *   - Changeset Create
   *   - Changeset Upload - responds with HTTP 200
   *   - Changeset Upload - responds with HTTP 200
   *   - Changeset Close
   */
  bool respond(HttpConnectionPtr& connection) override;
};

class ChangesetOutputThrottleTestServer : public HttpTestServer
{
public:
  /** Constructor */
  ChangesetOutputThrottleTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that responds to a series of OSM API requests
   *  to simulate a changeset upload.
   *  Requests, in order:
   *   - Capabilities
   *   - Permissions
   *   - Map Request - responds with HTTP 200
   *   - Changeset Create
   *   - Changeset Upload - responds with HTTP 200
   *   - Changeset Upload - responds with HTTP 200
   *   - Changeset Close
   */
  bool respond(HttpConnectionPtr& connection) override;
};

class ChangesetCreateFailureTestServer : public HttpTestServer
{
public:
  /** Constructor */
  ChangesetCreateFailureTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that responds to a series of OSM API requests
   *  to simulate a changeset create failure over and over.
   *  Requests, in order:
   *   - Capabilities
   *   - Permissions
   *   - Changeset Create Failure x6
   */
  bool respond(HttpConnectionPtr &connection) override;
};

class CreateWaysFailNodesTestServer : public HttpTestServer
{
public:
  /** Constructor */
  CreateWaysFailNodesTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that responds to a series of OSM API requests
   *  to simulate a changeset create failure over and over.
   *  Requests, in order:
   *   - Capabilities
   *   - Permissions
   *   - Changeset Create
   *   - Changeset Upload Failure - responds with HTTP 412
   *   - Changeset Upload - responds with HTTP 200
   *   - Changeset Close
   */
  bool respond(HttpConnectionPtr &connection) override;
};

class VersionFailureTestServer : public HttpTestServer
{
public:
  /** Constructor */
  VersionFailureTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that responds to a series of OSM API requests
   *  to simulate an element version failure over and over.
   *  Requests, in order:
   *   - Capabilities
   *   - Permissions
   *   - Changeset Create
   *   - Changeset Upload Version Failure - responds with HTTP 409
   *   - Element Get - responds with HTTP 200
   *   - Changeset Close
   */
  bool respond(HttpConnectionPtr &connection) override;
};

class ElementGoneTestServer : public HttpTestServer
{
public:
  /** Constructor */
  ElementGoneTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that responds to a series of OSM API requests
   *  to simulate an element gone test.
   *  Requests, in order:
   *   - Capabilities
   *   - Permissions
   *   - Changeset Create
   *   - Changeset Upload Gone Failure - responds with HTTP 410
   *   - Changeset Upload - responds with HTTP 200
   *   - Changeset Close
   */
  bool respond(HttpConnectionPtr &connection) override;
};

class ChangesetSplitDeleteTestServer : public HttpTestServer
{
public:
  /** Constructor */
  ChangesetSplitDeleteTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that responds to a series of OSM API requests
   *  to simulate a failed delete and then keep the changeset split deletes
   *  together.
   *  Requests, in order:
   *   - Capabilities
   *   - Permissions
   *   - Changeset Create
   *   - Changeset Upload Gone Failure - responds with HTTP 410
   *   - Changeset Upload - responds with HTTP 200
   *   - Changeset Upload - responds with HTTP 200
   *   - Changeset Close
   */
  bool respond(HttpConnectionPtr &connection) override;
};

class TimeoutTestServer : public HttpTestServer
{
public:
  /** Constructor */
  TimeoutTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that waits 5 seconds before responding to simulate a timeout
   */
  bool respond(HttpConnectionPtr &connection) override;
};

class OsmApiSampleRequestResponse
{
public:
  /** Sample Capabilities response body from '/api/0.6/capabilities'
   *  see: https://wiki.openstreetmap.org/wiki/API_v0.6#Capabilities:_GET_.2Fapi.2Fcapabilities
   */
  static const char* SAMPLE_CAPABILITIES_RESPONSE;
  /** Sample Permissions response body from '/api/0.6/permissions'
   *  see: https://wiki.openstreetmap.org/wiki/API_v0.6#Retrieving_permissions:_GET_.2Fapi.2F0.6.2Fpermissions
   */
  static const char* SAMPLE_PERMISSIONS_RESPONSE;
  /** Sample Changeset upload request body from '/api/0.6/changeset/#id/upload'
   *  see: https://wiki.openstreetmap.org/wiki/API_v0.6#Diff_upload:_POST_.2Fapi.2F0.6.2Fchangeset.2F.23id.2Fupload
   */
  static const char* SAMPLE_CHANGESET_REQUEST;
  /** Sample Changeset upload response body from '/api/0.6/changeset/1/upload' */
  static const char* SAMPLE_CHANGESET_1_RESPONSE;
  /** Sample map GET response from '/api/0.6/map' with the CGImap generator attribute */
  static const char* SAMPLE_CGIMAP_RESPONSE;
  /** Sample element GET response from '/api/0.6/way/1' */
  static const char* SAMPLE_ELEMENT_1_GET_RESPONSE;
  /** Sample Changeset upload response bodies from '/api/0.6/changeset/1/upload' divided into two responses */
  static const char* SAMPLE_CHANGESET_SUCCESS_1_RESPONSE;
  static const char* SAMPLE_CHANGESET_SUCCESS_2_RESPONSE;
  /** Sample Changeset upload response bodies for a failed response to '/api/0.6/changeset/1/upload' */
  static const char* SAMPLE_CHANGESET_FAILURE_RESPONSE_1;
  static const char* SAMPLE_CHANGESET_FAILURE_RESPONSE_2;
  /** Sample Changeset upload request */
  static const char* SAMPLE_CHANGESET_VERSION_FAILURE_CHANGESET;
  /** Sample Changeset upload conflict response body for a version conflict response to '/api/0.6/changeset/1/upload' */
  static const char* SAMPLE_CHANGESET_VERSION_FAILURE_RESPONSE;
  /** Sample Element GET response body for version conflict resolution */
  static const char* SAMPLE_CHANGESET_VERSION_FAILURE_GET_RESPONSE;
  /** Sample Element response bodies for a GONE response sequence to '/api/0.6/changeset/1/upload' */
  static const char* SAMPLE_ELEMENT_GONE_RESPONSE;
  /** Sample Changeset upload request for delete split test */
  static const char* SAMPLE_CHANGESET_SPLIT_DELETE;
  /** Sample Changeset upload responses for delete split test */
  static const char* SAMPLE_CHANGESET_SPLIT_FAILED_RESPONSE;
  static const char* SAMPLE_CHANGESET_SPLIT_SUCCESS_RESPONSE_1;
  static const char* SAMPLE_CHANGESET_SPLIT_SUCCESS_RESPONSE_2;
  static const char* SAMPLE_CHANGESET_SPLIT_SUCCESS_RESPONSE_3;
};

}

#endif  //  OSM_API_WRITER_TEST_SERVER_H
