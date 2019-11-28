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
#include "sqlite.hpp"
#include "export/rofi.hpp"

using namespace remwharead;
using std::string;

SCENARIO ("The Rofi export works correctly")
{
    bool exception = false;
    bool rofi_ok = true;

    GIVEN ("One database entry")
    {
        Database::entry entry;
        entry.uri = "https://example.com/page.html";
        entry.title = "Thoughtful title";
        entry.tags = { "tag1", "tag2" };

        const string expected = static_cast<char>(0x00) + string("markup-rows")
            + static_cast<char>(0x1f) + "true\n"
            "Thoughtful title "
            R"(<span size="small" weight="light" style="italic">(tag1,tag2))"
            R"(</span> <span size="xx-small" weight="ultralight">)"
            "https://example.com/page.html</span>\n";

        try
        {
            std::ostringstream output;
            Export::Rofi({ entry }, output).print();
            const string rofi = output.str();

            if (rofi != expected)
            {
                rofi_ok = false;
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
            REQUIRE(rofi_ok);
        }
    }
}
