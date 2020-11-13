#include "curl_wrapper.hpp"

#include <catch.hpp>

#include <exception>
#include <string>

namespace curl_wrapper
{

using std::string;

SCENARIO("HTTP GET", "[http]")
{
    const string uri{"https://schlomp.space/api/v1/version"};

    bool exception = false;
    string answer;

    WHEN("GETing " + uri)
    {
        try
        {
            CURLWrapper curl;
            answer = curl.make_http_request(http_method::GET, uri).body;
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN("No exception is thrown")
        AND_THEN("We get the right answer")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(answer.substr(0, 11) == R"({"version":)");
        }
    }
}

} // namespace curl_wrapper
