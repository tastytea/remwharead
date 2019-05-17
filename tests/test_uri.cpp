/*  This file is part of remwharead.
 *  Copyright © 2019 tastytea <tastytea@tastytea.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <exception>
#include <string>
#include <catch.hpp>
#include "uri.hpp"

using std::string;

SCENARIO ("URI works correctly")
{
    GIVEN ("A test class, child of URI")
    {
        class URITest : protected URI
        {
        public:
            explicit URITest(const string &)
                : URI("") {}
            URITest()
                : URI("test.html") {}

            bool test_title()
            {
                if (extract_title(_html) == "title")
                {
                    return true;
                }
                return false;
            }

            bool test_description()
            {
                if (extract_description(_html) == "description")
                {
                    return true;
                }
                return false;
            }

            bool test_fulltext()
            {
                if (strip_html(_html) == "titleA short sentence.")
                {
                    return true;
                }
                return false;
            }

        private:
            const string _html =
                "<html><head><title>title</title>"
                "<meta name=\"description\" content=\"description\" />"
                "<body><p>A short <span style=\"\">sentence</span>.</p>"
                "</body></head></html>";
        };

        WHEN ("extract_title() is called")
        {
            URITest testuri;

            THEN ("No exception is thrown")
                AND_THEN ("Output is as expected")
            {
                REQUIRE_NOTHROW(testuri.test_title());
                REQUIRE(testuri.test_title());
            }
        }

        WHEN ("extract_description() is called")
        {
            URITest testuri;

            THEN ("No exception is thrown")
                AND_THEN ("Output is as expected")
            {
                REQUIRE_NOTHROW(testuri.test_description());
                REQUIRE(testuri.test_description());
            }
        }

        WHEN ("extract_fulltext() is called")
        {
            URITest testuri;

            THEN ("No exception is thrown")
                AND_THEN ("Output is as expected")
            {
                REQUIRE_NOTHROW(testuri.test_fulltext());
                REQUIRE(testuri.test_fulltext());
            }
        }
    }
}
