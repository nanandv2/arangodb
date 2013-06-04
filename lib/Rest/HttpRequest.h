////////////////////////////////////////////////////////////////////////////////
/// @brief http request
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2013 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Achim Brandt
/// @author Copyright 2004-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_REST_HTTP_REQUEST_H
#define TRIAGENS_REST_HTTP_REQUEST_H 1

#include "Basics/Common.h"
#include "Basics/Dictionary.h"

#include "BasicsC/json.h"
#include "BasicsC/string-buffer.h"
#include "Basics/StringBuffer.h"
#include "Rest/ConnectionInfo.h"

using namespace std;

// -----------------------------------------------------------------------------
// --SECTION--                                                 class HttpRequest
// -----------------------------------------------------------------------------

namespace triagens {
  namespace rest {

////////////////////////////////////////////////////////////////////////////////
/// @brief http request
///
/// The http server reads the request string from the client and converts it
/// into an instance of this class. An http request object provides methods to
/// inspect the header and parameter fields.
////////////////////////////////////////////////////////////////////////////////

    class  HttpRequest {
      private:
        HttpRequest (HttpRequest const&);
        HttpRequest& operator= (HttpRequest const&);

// -----------------------------------------------------------------------------
// --SECTION--                                                      public types
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief http request type
////////////////////////////////////////////////////////////////////////////////

        enum HttpRequestType {
          HTTP_REQUEST_DELETE,
          HTTP_REQUEST_GET,
          HTTP_REQUEST_HEAD,
          HTTP_REQUEST_OPTIONS,
          HTTP_REQUEST_POST,
          HTTP_REQUEST_PUT,
          HTTP_REQUEST_PATCH,
          HTTP_REQUEST_ILLEGAL
        };

////////////////////////////////////////////////////////////////////////////////
/// @brief http version
////////////////////////////////////////////////////////////////////////////////

        enum HttpVersion {
          HTTP_1_0,
          HTTP_1_1
        };

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief http request constructor
///
/// Constructs a http request given nothing. You can add the values, the header
/// information, and path information afterwards.
////////////////////////////////////////////////////////////////////////////////

        HttpRequest ();

////////////////////////////////////////////////////////////////////////////////
/// @brief http request constructor
///
/// Constructs a http request given the header string. A client request
/// consists of two parts: the header and the body. For a GET request the
/// body is always empty and all information about the request is delivered
/// in the header. For a POST or PUT request some information is also
/// delivered in the body. However, it is necessary to parse the header
/// information, before the body can be read.
////////////////////////////////////////////////////////////////////////////////

        HttpRequest (char const* header, size_t length);

////////////////////////////////////////////////////////////////////////////////
/// @brief destructor
////////////////////////////////////////////////////////////////////////////////

        virtual ~HttpRequest ();

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief gets the request body as TRI_json_t*
////////////////////////////////////////////////////////////////////////////////

        TRI_json_t* toJson (char**);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the protocol
////////////////////////////////////////////////////////////////////////////////

        const string& protocol () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the connection info
////////////////////////////////////////////////////////////////////////////////

        void setProtocol (const string& protocol);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the connection info
////////////////////////////////////////////////////////////////////////////////

        ConnectionInfo const& connectionInfo () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the connection info
////////////////////////////////////////////////////////////////////////////////

        void setConnectionInfo (ConnectionInfo const& info);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the http request type
////////////////////////////////////////////////////////////////////////////////

        HttpRequestType requestType () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the full url of the request
////////////////////////////////////////////////////////////////////////////////

        const string& fullUrl () const {
          return _fullUrl;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the http request type
////////////////////////////////////////////////////////////////////////////////

        void setRequestType (HttpRequestType newType);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns whether HTTP version is 1.0
////////////////////////////////////////////////////////////////////////////////

        bool isHttp10 () const {
          return _version == HTTP_1_0;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief returns whether HTTP version is 1.1
////////////////////////////////////////////////////////////////////////////////

        bool isHttp11 () const {
          return _version == HTTP_1_1;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the full request path
///
/// The request path consists of the URL without the host and without any
/// parameters.
////////////////////////////////////////////////////////////////////////////////

        const char* requestPath () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief writes representation to string buffer
////////////////////////////////////////////////////////////////////////////////

        void write (TRI_string_buffer_t*) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the authenticated user
////////////////////////////////////////////////////////////////////////////////

        std::string const& user ();

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the authenticated user
////////////////////////////////////////////////////////////////////////////////

        void setUser (std::string const& user);

// -----------------------------------------------------------------------------
// --SECTION--                                      public prefix/suffix methods
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the prefix path of the request
///
/// The request path consists of the URL without the host and without any
/// parameters.  The request path is split into two parts: the prefix, namely
/// the part of the request path that was match by a handler and the suffix with
/// all the remaining arguments.
////////////////////////////////////////////////////////////////////////////////

        char const* prefix () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the prefix path of the request
////////////////////////////////////////////////////////////////////////////////

        void setPrefix (char const* path);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns all suffix parts
////////////////////////////////////////////////////////////////////////////////

        vector<string> const& suffix () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief adds a suffix part
////////////////////////////////////////////////////////////////////////////////

        void addSuffix (char const* part);

// -----------------------------------------------------------------------------
// --SECTION--                                             public header methods
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the content length
////////////////////////////////////////////////////////////////////////////////

        int64_t contentLength () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns a header field
///
/// Returns the value of a header field with given name. If no header field
/// with the given name was specified by the client, the empty string is
/// returned.
///
/// @note The @FA{field} must be lowercase.
////////////////////////////////////////////////////////////////////////////////

        char const* header (char const* key) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns a header field
///
/// Returns the value of a header field with given name. If no header field
/// with the given name was specified by the client, the empty string is
/// returned. found is try if the client specified the header field.
///
/// @note The @FA{field} must be lowercase.
////////////////////////////////////////////////////////////////////////////////

        char const* header (char const* key, bool& found) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns all header fields
///
/// Returns a copy of all header fields.
////////////////////////////////////////////////////////////////////////////////

        map<string, string> headers () const;

// -----------------------------------------------------------------------------
// --SECTION--                                              public value methods
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the value of a key
///
/// Returns the value of a key. The empty string is returned if key was not
/// specified by the client.
////////////////////////////////////////////////////////////////////////////////

        char const* value (char const* key) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the value of a key
///
/// Returns the value of a key. The empty string is returned if key was not
/// specified by the client. found is true if the client specified the key.
////////////////////////////////////////////////////////////////////////////////

        char const* value (char const* key, bool& found) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns all values
///
/// Returns all key/value pairs of the request.
////////////////////////////////////////////////////////////////////////////////

        map<string, string> values () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the value of an array key
///
/// Returns the value of a key. The empty string is returned if key was not
/// specified by the client.
////////////////////////////////////////////////////////////////////////////////

        vector<char const*> const* arrayValue (char const* key) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the value of an array key
///
/// Returns the value of a key. The empty string is returned if key was not
/// specified by the client. found is true if the client specified the key.
////////////////////////////////////////////////////////////////////////////////

        vector<char const*> const* arrayValue (char const* key, bool& found) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns all array values
///
/// Returns all key/value pairs of the request.
////////////////////////////////////////////////////////////////////////////////

        map<string, vector<char const*>* > arrayValues () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the value of a cookie
///
/// Returns the value of a cookie.
////////////////////////////////////////////////////////////////////////////////

        char const* cookieValue (char const* key) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief  returns the value of a cookie
///
/// Returns the value of a cookie. found is true if the client specified the key.
////////////////////////////////////////////////////////////////////////////////

        char const* cookieValue (char const* key, bool& found) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns all cookies
///
/// Returns all key/value pairs of the request.
////////////////////////////////////////////////////////////////////////////////

        map<string, string > cookieValues () const;

// -----------------------------------------------------------------------------
// --SECTION--                                               public body methods
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief gets the body
////////////////////////////////////////////////////////////////////////////////

        char const* body () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief gets the body size
////////////////////////////////////////////////////////////////////////////////

        size_t bodySize () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief register a copy of the body passed
////////////////////////////////////////////////////////////////////////////////

        int setBody (char const* newBody, size_t length);

////////////////////////////////////////////////////////////////////////////////
/// @brief set a header field
////////////////////////////////////////////////////////////////////////////////

        void setHeader (char const* key, size_t keyLength, char const* value);

// -----------------------------------------------------------------------------
// --SECTION--                                             public static methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief translate an enum value into an HTTP method string
////////////////////////////////////////////////////////////////////////////////

        static string translateMethod (const HttpRequestType);

////////////////////////////////////////////////////////////////////////////////
/// @brief translate an HTTP method string into an enum value
////////////////////////////////////////////////////////////////////////////////

        static HttpRequestType translateMethod (const string&);

////////////////////////////////////////////////////////////////////////////////
/// @brief append the request method string to a string buffer
////////////////////////////////////////////////////////////////////////////////

        static void appendMethod (HttpRequestType, triagens::basics::StringBuffer*);

////////////////////////////////////////////////////////////////////////////////
/// @brief the expected content-type for a subpart
////////////////////////////////////////////////////////////////////////////////

        static const string& getPartContentType ();

////////////////////////////////////////////////////////////////////////////////
/// @brief the expected content-type for a multipart message
////////////////////////////////////////////////////////////////////////////////

        static const string& getMultipartContentType ();

// -----------------------------------------------------------------------------
// --SECTION--                                                   private methods
// -----------------------------------------------------------------------------

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief determine the header type
////////////////////////////////////////////////////////////////////////////////

        static HttpRequestType getRequestType (const char*, const size_t);

////////////////////////////////////////////////////////////////////////////////
/// @brief parses the http header
////////////////////////////////////////////////////////////////////////////////

        void parseHeader (char* ptr, size_t length);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the full url of the request
////////////////////////////////////////////////////////////////////////////////

        void setFullUrl (char const* begin, char const* end);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the path of the request
///
/// @note The @FA{path} must exists as long as the instance is alive and it
///       must be garbage collected by the caller.
////////////////////////////////////////////////////////////////////////////////

        void setRequestPath (char const* path);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets a key/value pair
////////////////////////////////////////////////////////////////////////////////

        void setValue (char const* key, char const* value);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the header values
////////////////////////////////////////////////////////////////////////////////

        void setValues (char* buffer, char* end);

////////////////////////////////////////////////////////////////////////////////
/// @brief set array value
////////////////////////////////////////////////////////////////////////////////

        void setArrayValue (char* key, size_t length, char const* value);

////////////////////////////////////////////////////////////////////////////////
/// @brief set cookie
////////////////////////////////////////////////////////////////////////////////

        void setCookie (char* key, size_t length, char const* value);

////////////////////////////////////////////////////////////////////////////////
/// @brief parse value of a cookie header field
////////////////////////////////////////////////////////////////////////////////

        void parseCookies (const char* buffer);

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief complete request path, without protocol, host, and parameters
////////////////////////////////////////////////////////////////////////////////

        char const* _requestPath;

////////////////////////////////////////////////////////////////////////////////
/// @brief headers
////////////////////////////////////////////////////////////////////////////////

        basics::Dictionary<char const*> _headers;

////////////////////////////////////////////////////////////////////////////////
/// @brief values
////////////////////////////////////////////////////////////////////////////////

        basics::Dictionary<char const*> _values;

////////////////////////////////////////////////////////////////////////////////
/// @brief array values
////////////////////////////////////////////////////////////////////////////////

        basics::Dictionary< vector<char const*>* > _arrayValues;

////////////////////////////////////////////////////////////////////////////////
/// @brief cookies
////////////////////////////////////////////////////////////////////////////////

        basics::Dictionary<char const*> _cookies;

////////////////////////////////////////////////////////////////////////////////
/// @brief content length
////////////////////////////////////////////////////////////////////////////////

        int64_t _contentLength;

////////////////////////////////////////////////////////////////////////////////
/// @brief body
////////////////////////////////////////////////////////////////////////////////

        char* _body;

////////////////////////////////////////////////////////////////////////////////
/// @brief body size
////////////////////////////////////////////////////////////////////////////////

        size_t _bodySize;

////////////////////////////////////////////////////////////////////////////////
/// @brief list of memory allocated which will be freed in the destructor
////////////////////////////////////////////////////////////////////////////////

        vector<char*> _freeables;

////////////////////////////////////////////////////////////////////////////////
/// @brief the protocol used
////////////////////////////////////////////////////////////////////////////////

        string _protocol;

////////////////////////////////////////////////////////////////////////////////
/// @brief connection info for the server and the peer
////////////////////////////////////////////////////////////////////////////////

        ConnectionInfo _connectionInfo;

////////////////////////////////////////////////////////////////////////////////
/// @brief the request type
////////////////////////////////////////////////////////////////////////////////

        HttpRequestType _type;

////////////////////////////////////////////////////////////////////////////////
/// @brief the prefix of the request path
////////////////////////////////////////////////////////////////////////////////

        string _prefix;

////////////////////////////////////////////////////////////////////////////////
/// @brief the full url requested
////////////////////////////////////////////////////////////////////////////////

        string _fullUrl;

////////////////////////////////////////////////////////////////////////////////
/// @brief the suffixes for the request path
////////////////////////////////////////////////////////////////////////////////

        vector<string> _suffix;

////////////////////////////////////////////////////////////////////////////////
/// @brief the HTTP version
////////////////////////////////////////////////////////////////////////////////

        HttpVersion _version;

////////////////////////////////////////////////////////////////////////////////
/// @brief authenticated user
////////////////////////////////////////////////////////////////////////////////

        string _user;
    };
  }
}

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
