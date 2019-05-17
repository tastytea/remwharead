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
#include <sstream>
#include <regex>
#include <catch.hpp>
#include "time.hpp"
#include "sqlite.hpp"
#include "export.hpp"

using std::string;
using std::chrono::system_clock;
using std::regex;
using std::regex_search;

SCENARIO ("The CSV export works correctly")
{
    bool exception = false;
    bool csv_ok = false;

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
            export_csv({ entry }, output);
            const string csv = output.str();

            const regex re
                ("\"URI\",\"Archived URI\",\"Date & time\",\"Tags\","
                "\"Title\",\"Description\",\"Full text\"\r\n"
                "\"https://example.com/page.html\",\"\","
                "\"1970-01-01T\\d{2}:\\d{2}:\\d{2}\",\"tag1,tag2\","
                 "\"Nice title\",\"Good description.\",\"Full text.\"\r\n");
            if (regex_search(csv, re))
            {
                csv_ok = true;
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
            REQUIRE(csv_ok);
        }
    }
}
