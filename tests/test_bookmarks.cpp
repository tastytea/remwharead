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
#include "bookmarks.hpp"

using std::string;
using std::chrono::system_clock;
using std::regex;
using std::regex_search;

SCENARIO ("The Bookmarks export works correctly")
{
    bool exception = false;
    bool bookmarks_ok = true;

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
            Export::Bookmarks({ entry }, output).print();
            const string bookmarks = output.str();

            const regex re_top(
                "^<!DOCTYPE NETSCAPE-Bookmark-file-1>\n"
                "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; "
                "charset=UTF-8\">\n"
                "<TITLE>Bookmarks from remwharead</TITLE>\n"
                "<H1>Bookmarks from remwharead<H1>\n\n"
                "<DL><p>\n<DT><H3>remwharead</H3>\n<DL><p>");
            const regex re_bottom(
                "<DT><A HREF=\"https://example.com/page.html\" "
                "ADD_DATE=\"\\d+\">Nice title</A>\n"
                "</DL><p>\n</DL><p>\n");

            for (const regex &re : { re_top, re_bottom })
            {
                if (!regex_search(bookmarks, re))
                {
                    bookmarks_ok = false;
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
            REQUIRE(bookmarks_ok);
        }
    }
}
