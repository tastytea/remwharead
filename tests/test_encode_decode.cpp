#include "curl_wrapper.hpp"

#include <catch.hpp>

#include <exception>
#include <string>

namespace curl_wrapper
{

using std::string;

SCENARIO("URL encoding / decoding")
{
    const string text_raw{"Hüpfburg am rande!"};
    const string text_escaped("H%C3%BCpfburg%20am%20rande%21");

    bool exception = false;
    string answer;

    WHEN("Encoding " + text_raw)
    {
        try
        {
            CURLWrapper curl;
            answer = curl.escape_url(text_raw);
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN("No exception is thrown")
        AND_THEN("The text is successfully encoded")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(answer == text_escaped);
        }
    }

    WHEN("Decoding " + text_escaped)
    {
        try
        {
            CURLWrapper curl;
            answer = curl.unescape_url(text_escaped);
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN("No exception is thrown")
        AND_THEN("The text is successfully decoded")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(answer == text_raw);
        }
    }
}

} // namespace curl_wrapper
