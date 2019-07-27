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
#include <sstream>
#include <regex>
#include <chrono>
#include <catch.hpp>
#include "sqlite.hpp"
#include "export/simple.hpp"

using namespace remwharead;
using std::string;
using std::chrono::system_clock;
using std::regex;
using std::regex_search;

SCENARIO ("The Simple export works correctly")
{
    bool exception = false;
    bool simple_ok = true;

    GIVEN ("One database entry")
    {
        Database::entry entry;
        entry.uri = "https://example.com/page.html";
        entry.tags = { "tag1", "tag2" };
        entry.title = "Nice title";
        entry.datetime = system_clock::time_point();
        entry.fulltext = "Full text.";
        entry.description = "Good description.";

        try
        {
            std::ostringstream output;
            Export::Simple({ entry }, output).print();
            const string simple = output.str();

            const regex re("[0-9]{4}-[0-9]{2}-[0-9]{2}: Nice title\n"
                           " {12}<https://example\\.com/page\\.html>\n");

            if (!regex_search(simple, re))
            {
                simple_ok = false;
            }
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN ("No exception is thrown")
            AND_THEN ("Output looks okay")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(simple_ok);
        }
    }
}
