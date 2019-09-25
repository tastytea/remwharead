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

#ifndef REMWHAREAD_TIME_HPP
#define REMWHAREAD_TIME_HPP

#include <string>
#include <chrono>

//! @file

namespace remwharead
{
    using std::string;
    using std::chrono::system_clock;
    using time_point = system_clock::time_point;

    /*!
     *  @brief  Convert ISO 8601 or SQLite time-string to time_point.
     *
     *  The SQLite format is *YY-MM-DD hh:mm:ss* instead of *YY-MM-DDThh:mm:ss*.
     *
     *  @param  strtime Time string in ISO 8601 or SQLite format.
     *  @param  sqlite  Is the string in SQLite format?
     */
    time_point string_to_timepoint(const string &strtime, bool sqlite = false);

    /*!
     *  @brief  Convert time_point to ISO 8601 or SQLite time-string.
     *
     *  The SQLite format is *YY-MM-DD hh:mm:ss* instead of *YY-MM-DDThh:mm:ss*.
     *
     *  @param  time_point The std::chrono::system_clock::time_point.
     *  @param  sqlite     Is the string in SQLite format?
     */
    string timepoint_to_string(const time_point &tp, bool sqlite = false);
} // namespace remwharead

#endif  // REMWHAREAD_TIME_HPP
