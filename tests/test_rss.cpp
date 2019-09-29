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
#include "export/rss.hpp"

using namespace remwharead;
using std::string;
using std::chrono::system_clock;
using std::regex;
using std::regex_search;

SCENARIO ("The RSS export works correctly")
{
    bool exception = false;
    bool rss_ok = true;

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
            Export::RSS({ entry }, output).print();
            const string rss = output.str();

            const regex re(
                R"(^<rss version="2\.0" )"
                R"(xmlns:atom="http://www\.w3\.org/2005/Atom"><channel>)"
                R"(<title>Visited things</title><link/>)"
                R"(<description>Export from remwharead\.</description>)"
                R"(<generator>remwharead \d+\.\d+\.\d+</generator>)"
                R"(<lastBuildDate>[A-Z][a-z]{2}, \d{2} [A-Z][a-z]{2} \d{4} )"
                R"((\d{2}:){2}\d{2} [A-Z]+</lastBuildDate>)"
                R"(<item><title>Nice title</title>)"
                R"(<link>https://example\.com/page\.html</link>)"
                R"(<guid isPermaLink="false">https://example\.com/page\.html )"
                R"(at \d{4}(-\d{2}){2}T(\d{2}:){2}\d{2}</guid>)"
                R"(<pubDate>[A-Z][a-z]{2}, \d{2} [A-Z][a-z]{2} \d{4} )"
                R"((\d{2}:){2}\d{2} [A-Z]+</pubDate>)"
                R"(<description>&lt;p&gt;Good description\.&lt;/p&gt;)"
                R"(&lt;p&gt;&lt;strong&gt;Tags:&lt;/strong&gt; )"
                R"(tag1, tag2&lt;/p&gt;</description>)"
                R"(</item></channel></rss>\n$)");

            if (!regex_search(rss, re))
            {
                rss_ok = false;
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
            REQUIRE(rss_ok);
        }
    }
}
