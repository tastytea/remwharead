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
#include <vector>
#include <catch.hpp>
#include "parse_options.hpp"

using std::string;
using std::vector;

SCENARIO ("The option parser works correctly")
{
    bool exception = false;
    options opts;
    const string url = "https://example.com/article.html";

    WHEN ("The options are --help --file test")
    {
        try
        {
            const char *argv[]
                = { "remwharead", "--help", "--file", "test" };
            opts = parse_options(4, argv);
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN ("No exception is thrown")
            AND_THEN ("status code is 0")
            AND_THEN ("options.file is empty")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(opts.status_code == 0);
            REQUIRE(opts.file == "");
        }
    }

    WHEN ("The options are --version --file test")
    {
        try
        {
            const char *argv[]
                = { "remwharead", "--version", "--file", "test" };
            opts = parse_options(4, argv);
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN ("No exception is thrown")
            AND_THEN ("status code is 0")
            AND_THEN ("options.file is empty")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(opts.status_code == 0);
            REQUIRE(opts.file == "");
        }
    }

    WHEN ("The options are -t 💩 " + url)
    {
        try
        {
            const char *argv[]
                = { "remwharead", "-t", "💩", url.c_str() };
            opts = parse_options(4, argv);
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN ("No exception is thrown")
            AND_THEN ("status code is 0")
            AND_THEN ("options.file is empty")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(opts.status_code == 0);
            REQUIRE(opts.tags == vector<string>{ "💩" });
            REQUIRE(opts.url == url);
        }
    }

    WHEN ("A very long string is passed as a tag")
    {
        const string longstring = // 5 · 60 = 300
            "aw6hui6chieRo9aihai9un1aeke6oushoo2oRo4aeD6eiDeiSheek4ahGiel"
            "othaemeeyo4ievieV8kae9xiriejohD0aelah6oophaQueilohyaix3joo7O"
            "laiceeFePuetaeBe0aip3eemuaheer0aj8aij8ahchisi4eiperiechoopoo"
            "ohmie5doog5ohbahDoodah7daurah6haebeife2tah5Pheeweeb0eishooc4"
            "phohKu5Ha3HiCeedeoph1ocaingaHeedeepeesohmee6Equ4meirahk3aihe";
        const string tags = "tag1," + longstring + ",tag3";
        try
        {
            const char *argv[] =
                {
                    "remwharead",
                    "-t",
                    tags.c_str(),
                    url.c_str()
                };
            opts = parse_options(4, argv);
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN ("No exception is thrown")
            AND_THEN ("status code is 0")
            AND_THEN ("options.file is empty")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(opts.status_code == 0);
            REQUIRE(opts.tags == vector<string>{ "tag1", longstring, "tag3" });
            REQUIRE(opts.url == url);
        }
    }
}
