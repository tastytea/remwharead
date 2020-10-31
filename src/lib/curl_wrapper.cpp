#include "curl_wrapper.hpp"

#include "version.hpp"

#include <Poco/RegularExpression.h>
#include <curl/curl.h>

#include <atomic>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace remwharead
{

using std::runtime_error;
using std::to_string;
using std::vector;
using RegEx = Poco::RegularExpression;

static std::atomic<bool> initialized{false};

CURLWrapper::CURLWrapper()
{
    CURLcode code{CURLE_OK};
    if (!initialized)
    {
        // NOLINTNEXTLINE(hicpp-signed-bitwise)
        code = curl_global_init(CURL_GLOBAL_ALL);
        initialized = true;
    }
    _connection = curl_easy_init();
    if (_connection == nullptr || code != CURLE_OK)
    {
        throw runtime_error{"Failed to initialize curl. libcurl code: "
                            + to_string(code)};
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
    code = curl_easy_setopt(_connection, CURLOPT_FOLLOWLOCATION, 1L);
    if (code != CURLE_OK)
    {
        throw runtime_error{"HTTP is not supported."};
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_MAXREDIRS, 5L);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    code = curl_easy_setopt(_connection, CURLOPT_USERAGENT,
                            (string("remwharead/") += version).c_str());
    if (code != CURLE_OK)
    {
        throw runtime_error{"Failed to set User-Agent."};
    }
}

CURLWrapper::~CURLWrapper() noexcept
{
    curl_easy_cleanup(_connection);
}

string CURLWrapper::make_request(string uri, bool archive)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    CURLcode code{curl_easy_setopt(_connection, CURLOPT_URL, uri.c_str())};
    if (code != CURLE_OK)
    {
        throw runtime_error{"Couldn't set URL: " + to_string(code)};
    }

    if (archive)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(_connection, CURLOPT_CUSTOMREQUEST, "HEAD");
    }
    else
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(_connection, CURLOPT_HTTPGET, 1L);
    }

    code = curl_easy_perform(_connection);
    if (code != CURLE_OK)
    {
        // I think PARTIAL_FILE is normal for HEAD requests?
        if (archive && code != CURLE_PARTIAL_FILE)
        {
            throw runtime_error{"libcurl error: " + to_string(code)};
        }
    }

    long http_status{0}; // NOLINT(google-runtime-int)
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_getinfo(_connection, CURLINFO_RESPONSE_CODE, &http_status);

    if (http_status == 200)
    {
        if (archive)
        {
            const RegEx re_location(R"(^Content-Location:\s*(.+)$)",
                                    RegEx::RE_CASELESS);
            vector<string> matches;
            re_location.split(_buffer_headers, matches);
            if (matches.size() >= 2)
            {
                if (!matches[1].empty())
                {
                    return matches[1];
                }
            }
            return uri;
        }
        return _buffer_body;
    }

    throw runtime_error{"HTTP error: " + to_string(http_status)};
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

} // namespace remwharead
