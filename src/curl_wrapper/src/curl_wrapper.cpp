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

#include "curl_wrapper.hpp"

#include "types.hpp"

#include <curl/curl.h>

#include <atomic>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <string>

namespace curl_wrapper
{

inline static std::atomic<std::uint64_t> curlwrapper_instances{0};

CURLWrapper::CURLWrapper()
{
    if (curlwrapper_instances == 0)
    {
        // NOLINTNEXTLINE(hicpp-signed-bitwise)
        check(curl_global_init(CURL_GLOBAL_ALL));
    }
    ++curlwrapper_instances;

    _connection = curl_easy_init();
    if (_connection == nullptr)
    {
        throw std::runtime_error{"Failed to initialize curl."};
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_ERRORBUFFER, _buffer_error);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_WRITEFUNCTION, writer_body_wrapper);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_WRITEDATA, this);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_HEADERFUNCTION,
                     writer_headers_wrapper);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_HEADERDATA, this);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    check(curl_easy_setopt(_connection, CURLOPT_FOLLOWLOCATION, 1L));
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_MAXREDIRS, 5L);
}

CURLWrapper::~CURLWrapper() noexcept
{
    curl_easy_cleanup(_connection);
    --curlwrapper_instances;

    if (curlwrapper_instances == 0)
    {
        curl_global_cleanup();
    }
}

string CURLWrapper::escape_url(const string_view url) const
{
    char *cbuf{curl_easy_escape(_connection, url.data(),
                                static_cast<int>(url.size()))};
    string sbuf{cbuf};
    curl_free(cbuf);
    return sbuf;
}

string CURLWrapper::unescape_url(const string_view url) const
{
    char *cbuf{curl_easy_unescape(_connection, url.data(),
                                  static_cast<int>(url.size()), nullptr)};
    string sbuf{cbuf};
    curl_free(cbuf);
    return sbuf;
}

void CURLWrapper::set_useragent(const string_view useragent)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    check(curl_easy_setopt(_connection, CURLOPT_USERAGENT, useragent.data()));
}

void CURLWrapper::set_proxy(const string_view proxy)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    check(curl_easy_setopt(_connection, CURLOPT_PROXY, proxy.data()));
}

answer CURLWrapper::make_http_request(http_method method, string_view uri)
{
    _buffer_headers.clear();
    _buffer_body.clear();

    switch (method)
    {
    case http_method::DELETE:
    {
        // NOTE: Use CURLOPT_MIMEPOST, then set to DELETE to send data.

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        check(curl_easy_setopt(_connection, CURLOPT_CUSTOMREQUEST, "DELETE"));

        break;
    }
    case http_method::GET:
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(_connection, CURLOPT_HTTPGET, 1L);

        break;
    }
    case http_method::HEAD:
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        check(curl_easy_setopt(_connection, CURLOPT_CUSTOMREQUEST, "HEAD"));

        break;
    }
    case http_method::PATCH:
    {
        // NOTE: Use CURLOPT_MIMEPOST, then set to PATCH to send data.

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        check(curl_easy_setopt(_connection, CURLOPT_CUSTOMREQUEST, "PATCH"));

        break;
    }
    case http_method::POST:
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(_connection, CURLOPT_POST, 1L);

        // NOTE: Use CURLOPT_MIMEPOST to send data.

        break;
    }
    case http_method::PUT:
    {
        // NOTE: Use CURLOPT_MIMEPOST, then set to PUT to send data.

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        check(curl_easy_setopt(_connection, CURLOPT_CUSTOMREQUEST, "PUT"));

        break;
    }
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    check(curl_easy_setopt(_connection, CURLOPT_URL, uri.data()));

    try
    {
        check(curl_easy_perform(_connection));
    }
    catch (const CURLException &e)
    {
        // PARTIAL_FILE error seems to be normal for HEAD requests.
        if (!(method == http_method::HEAD
              && e.error_code == CURLE_PARTIAL_FILE))
        {
            std::rethrow_exception(std::current_exception());
        }
    }

    long http_status{0}; // NOLINT(google-runtime-int)
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    check(curl_easy_getinfo(_connection, CURLINFO_RESPONSE_CODE, &http_status));
    return {static_cast<std::uint16_t>(http_status), _buffer_headers,
            _buffer_body};
}

size_t CURLWrapper::writer_body(char *data, size_t size, size_t nmemb)
{
    if (data == nullptr)
    {
        return 0;
    }

    _buffer_body.append(data, size * nmemb);

    return size * nmemb;
}

size_t CURLWrapper::writer_headers(char *data, size_t size, size_t nmemb)
{
    if (data == nullptr)
    {
        return 0;
    }

    _buffer_headers.append(data, size * nmemb);

    return size * nmemb;
}

void CURLWrapper::check(const CURLcode code)
{
    if (code != CURLE_OK)
    {
        throw CURLException{code, _buffer_error};
    }
}

const char *CURLException::what() const noexcept
{
    // NOTE: The string has to be static, or it'll vanish before it can be
    //       used. Couldn't find good documentation on that.
    static string error_string;
    error_string = _error_message;
    if (!error_string.empty())
    {
        error_string = " – " + error_string;
    }
    error_string = "libcurl error: " + std::to_string(error_code)
                   + error_string;
    return error_string.c_str();
}

} // namespace curl_wrapper
