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

SCENARIO ("The AsciiDoc export works correctly")
{
    bool exception = false;
    bool adoc_ok = true;

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
            Export::AsciiDoc({ entry }, output).print();
            const string adoc = output.str();

            const regex re_header
                ("^= Visited things\n"
                 ":Author: +remwharead \\d+\\.\\d+\\.\\d+\n"
                 ":Date: +\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}\n"
                 ":TOC: +right\n"
                 ":TOCLevels: +2\n\n");
            const regex re_dates
                ("== 1970-01-01\n\n"
                 "\\[\\[dt_1970-01-01T\\d{2}:\\d{2}:\\d{2}\\]\\]\n"
                 "\\* link:https://example\\.com/page\\.html\\[Nice title\\]"
                 " \\+\n_\\d{2}:\\d{2}_\n"
                 "| xref:t_tag1\\[tag1\\], xref:t_tag2\\[tag2\\] +\n"
                 "Good description\\.\n");
            const regex re_tags
                ("== Tags\n\n"
                 "=== Less used tags\n\n"
                 "==== \\[\\[t_tag1\\]\\]tag1\n\n"
                 "\\* xref:dt_1970-01-01T\\d{2}:\\d{2}:\\d{2}"
                 "\\[Nice title\\] _\\(1970-01-01\\)_\n\n");

            for (const regex &re : { re_header, re_dates, re_tags })
            {
                if (!regex_search(adoc, re))
                {
                    adoc_ok = false;
                    break;
                }
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
            REQUIRE(adoc_ok);
        }
    }
}
