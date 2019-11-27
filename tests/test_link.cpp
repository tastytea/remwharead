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
#include <catch.hpp>
#include "sqlite.hpp"
#include "export/link.hpp"

using namespace remwharead;
using std::string;

SCENARIO ("The Link export works correctly")
{
    bool exception = false;
    bool link_ok = true;

    GIVEN ("One database entry")
    {
        Database::entry entry;
        entry.uri = "https://example.com/page.html";

        try
        {
            std::ostringstream output;
            Export::Link({ entry }, output).print();
            const string link = output.str();

            if (link != (entry.uri + '\n'))
            {
                link_ok = false;
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
            REQUIRE(link_ok);
        }
    }
}
