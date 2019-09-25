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

#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <array>
#include "time.hpp"

namespace remwharead
{
    using std::array;

    time_point string_to_timepoint(const string &strtime, bool sqlite)
    {
        std::stringstream sstime(strtime);
        struct std::tm tm = {};
        tm.tm_isdst = -1;           // Detect daylight saving time.
        if (sqlite)
        {
            sstime >> std::get_time(&tm, "%Y-%m-%d %T");
        }
        else
        {
            sstime >> std::get_time(&tm, "%Y-%m-%dT%T");
        }
        std::time_t time = timelocal(&tm); // Assume time is local.
        return system_clock::from_time_t(time);
    }

    string timepoint_to_string(const time_point &tp, bool sqlite)
    {
        constexpr std::uint16_t bufsize = 32;
        std::time_t time = system_clock::to_time_t(tp);
        std::tm *tm;
        tm = std::localtime(&time);

        array<char, bufsize> buffer = {};

        if (sqlite)
        {
            std::strftime(buffer.begin(), bufsize, "%F %T", tm);
        }
        else
        {
            std::strftime(buffer.begin(), bufsize, "%FT%T", tm);
        }

        return buffer.begin();
    }
} // namespace remwharead
