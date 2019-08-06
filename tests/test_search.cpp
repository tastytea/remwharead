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
#include <chrono>
#include <vector>
#include <catch.hpp>
#include "sqlite.hpp"
#include "search.hpp"

using namespace remwharead;
using std::string;
using std::chrono::system_clock;
using std::vector;

SCENARIO ("Searching works correctly")
{
    bool exception = false;
    bool search_ok = true;

    Database::entry entry;
    entry.uri = "https://example.com/page.html";
    entry.tags = { "tag1_Ö", "tag2" };
    entry.title = "Nice title";
    entry.datetime = system_clock::time_point();
    entry.fulltext = "Full text.";
    entry.description = "Good description.";
    Search search({ entry });

    WHEN ("Searching in tags")
    {
        try
        {
            if (search.search_tags("tAg1_Ö", false).size() != 1)
            {
                search_ok = false;
            }

            if (search.search_tags( "tAg?[0-9]_ö", true).size() != 1)
            {
                search_ok = false;
            }
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN ("No exception is thrown")
            AND_THEN ("Results look okay")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(search_ok);
        }
    }

    WHEN ("Searching in everything")
    {
        try
        {
            if (search.search_all("DESCRIPT AND good", false).size() != 1)
            {
                search_ok = false;
            }

            if (search.search_all("^ful{2} T..T\\.$", true).size() != 1)
            {
                search_ok = false;
            }
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN ("No exception is thrown")
            AND_THEN ("Results look okay")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(search_ok);
        }
    }
}
