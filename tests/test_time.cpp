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
#include <catch.hpp>
#include "time.hpp"

using namespace remwharead;
using std::string;
using std::chrono::system_clock;
using std::chrono::seconds;

SCENARIO ("The time conversion works correctly")
{
    bool exception = false;
    const string datetime = "1970-01-04T00:00:00";
    system_clock::time_point tp;
    seconds seconds_since_epoch;

    GIVEN ("The date and time " + datetime + " as string")
    {
        try
        {
            tp = string_to_timepoint(datetime);
            seconds_since_epoch = std::chrono::duration_cast<seconds>
                (tp.time_since_epoch());
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN ("No exception is thrown")
            AND_THEN ("Seconds since epoch is not 0")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(seconds_since_epoch != seconds(0));
        }

        WHEN ("Converted back to string")
        {
            string datetime2;
            try
            {
                datetime2 = timepoint_to_string(tp);
            }
            catch (const std::exception &e)
            {
                exception = true;
            }

            THEN ("No exception is thrown")
                AND_THEN ("Date and time is the same as the initial value")
            {
                REQUIRE_FALSE(exception);
                REQUIRE(datetime2 == datetime);
            }
        }
    }
}
