#include "types.hpp"

#include <catch.hpp>

#include <exception>
#include <iostream>
#include <locale>
#include <string>

namespace curl_wrapper
{

using std::string;

SCENARIO("Extract header")
{
    answer ret;
    ret.headers = R"(HTTP/1.1 200 OK
Server: nginx/1.18.0
Date: Sat, 07 Nov 2020 22:26:13 GMT
Content-Type: application/rss+xml; charset=utf-8
Connection: keep-alive
Keep-Alive: timeout=20
Expires: Sat, 07 Nov 2020 22:56:13 GMT
Cache-Control: max-age=1800
X-Cache: HIT
X-UmläÜt: 🙂
)";
    bool exception = false;
    string value;

    WHEN("We search for “cache-control”")
    {
        try
        {
            value = ret.get_header("cache-control");
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception: " << e.what() << '\n';
            exception = true;
        }

        THEN("No exception is thrown")
        AND_THEN("The value is successfully extracted")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(value == "max-age=1800");
        }
    }

    // WHEN("We search for “X-UMLÄÜT”")
    // {
    //     std::locale::global(std::locale("de_DE.UTF-8"));
    //
    //     try
    //     {
    //         value = ret.get_header("X-UMLÄÜT");
    //     }
    //     catch (const std::exception &e)
    //     {
    //         std::cerr << "Exception: " << e.what() << '\n';
    //         exception = true;
    //     }

    //     THEN("No exception is thrown")
    //     AND_THEN("The value is successfully extracted")
    //     {
    //         REQUIRE_FALSE(exception);
    //         REQUIRE(value == "🙂");
    //     }
    // }
}

} // namespace curl_wrapper
