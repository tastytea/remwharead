/*  This file is part of curl_wrapper.
 *  Copyright © 2020 tastytea <tastytea@tastytea.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CURL_WRAPPER_HPP
#define CURL_WRAPPER_HPP

#include "types.hpp"

#include <curl/curl.h>

#include <exception>
#include <string>
#include <string_view>

namespace curl_wrapper
{

using std::string;
using std::string_view;

/*!
 *  @brief  Light wrapper around libcurl.
 *
 *  @since  0.1.0
 */
class CURLWrapper
{
public:
    /*!
     *  @brief  Initializes curl and sets up connection.
     *
     *  The first time an instance of CURLWrapper is created, it calls
     *  `curl_global_init`, which is not thread-safe. For more information
     *  consult [curl_global_init(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_global_init.html).
     *
     *  May throw CURLException or std::runtime_error.
     *
     *  @since  0.1.0
     */
    CURLWrapper();

    /*!
     *  @brief  Cleans up curl and connection.
     *
     *  Calls `curl_global_cleanup`, which is not thread-safe, when the last
     *  instance of CURLWrapper is destroyed. For more information consult
     *  [curl_global_cleanup(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_global_cleanup.html).
     *
     *  @since  0.1.0
     */
    virtual ~CURLWrapper() noexcept;

    //! Copy constructor. @since  0.1.0
    CURLWrapper(const CURLWrapper &other) = delete;

    //! Move constructor @since 0.1.0
    CURLWrapper(CURLWrapper &&other) noexcept = delete;

    //! Copy assignment operator @since  0.1.0
    CURLWrapper &operator=(const CURLWrapper &other) = delete;

    //! Move assignment operator @since  0.1.0
    CURLWrapper &operator=(CURLWrapper &&other) noexcept = delete;

    /*!
     *  @brief  Returns pointer to the CURL easy handle.
     *
     *  You can use this handle to set or modify curl options. For more
     *  information consult [curl_easy_setopt(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_easy_setopt.html).
     *
     *  @since  0.1.0
     */
    [[nodiscard]] inline CURL *get_curl_easy_handle() const
    {
        return _connection;
    }

    /*!
     *  @brief  URL encodes the given string.
     *
     *  For more information consult [curl_easy_escape(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_easy_escape.html).
     *
     *  @param  url String to escape.
     *
     *  @return The escaped string or {} if it failed.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] string escape_url(string_view url) const;

    /*!
     *  @brief  URL decodes the given string.
     *
     *  For more information consult [curl_easy_unescape(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_easy_unescape.html).
     *
     *  @param  url String to unescape.
     *
     *  @return The unescaped string or {} if it failed.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] string unescape_url(string_view url) const;

    /*!
     *  @brief  Set the User-Agent.
     *
     *  May throw CURLException.
     *
     *  @since  0.1.0
     */
    void set_useragent(string_view useragent);

    /*!
     *  @brief  Set a proxy.
     *
     *  For more information consult [CURLOPT_PROXY(3)]
     *  (https://curl.haxx.se/libcurl/c/CURLOPT_PROXY.html).
     *
     *  May throw CURLException.
     *
     *  @since  0.1.1
     */
    void set_proxy(string_view proxy);

    /*!
     *  @brief  Make a HTTP request.
     *
     *  May throw CURLException.
     *
     *  @param  method The HTTP method.
     *  @param  uri    The full URI.
     *
     *  @return The status code, headers and body of the page.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] answer make_http_request(http_method method, string_view uri);

private:
    CURL *_connection{};
    char _buffer_error[CURL_ERROR_SIZE]{};
    string _buffer_headers;
    string _buffer_body;

    /*!
     *  @brief  libcurl write callback function.
     *
     *  @since  0.1.0
     */
    size_t writer_body(char *data, size_t size, size_t nmemb);

    /*!
     *  @brief  Wrapper for curl, because it can only call static member
     *          functions.
     *
     *  <https://curl.haxx.se/docs/faq.html#Using_C_non_static_functions_f>
     *
     *  @since  0.1.0
     */
    static inline size_t writer_body_wrapper(char *data, size_t sz,
                                             size_t nmemb, void *f)
    {
        return static_cast<CURLWrapper *>(f)->writer_body(data, sz, nmemb);
    }

    //! @copydoc writer_body
    size_t writer_headers(char *data, size_t size, size_t nmemb);

    //! @copydoc writer_body_wrapper
    static inline size_t writer_headers_wrapper(char *data, size_t sz,
                                                size_t nmemb, void *f)
    {
        return static_cast<CURLWrapper *>(f)->writer_headers(data, sz, nmemb);
    }

    /*!
     *  @brief  Throw CURLException if command doesn't return CURLE_OK.
     *
     *  @since  0.1.0
     */
    void check(CURLcode code);
};

/*!
 *  @brief  Exception for libcurl errors.
 *
 *  @since  0.1.0
 */
class CURLException : public std::exception
{
public:
    /*!
     *  @brief  Constructor with error code.
     *
     *  @since  0.1.0
     */
    explicit CURLException(const CURLcode code)
        : error_code{code}
    {}

    /*!
     *  @brief Constructor with error code and error buffer.
     *
     *  @since  0.1.0
     */
    explicit CURLException(const CURLcode code, string_view error_buffer)
        : error_code{code}
        , _error_message{error_buffer}
    {}

    const CURLcode error_code; //!< Error code from libcurl.

    /*!
     *  @brief  Error message.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] const char *what() const noexcept override;

private:
    string _error_message;
};

} // namespace curl_wrapper

#endif // CURL_WRAPPER_HPP
