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

using std::string;
using std::chrono::system_clock;

// Convert ISO 8601 time-string to time_point.
const system_clock::time_point string_to_timepoint(const string &strtime);

// Convert time_point to USO 8601 time-string.
const string timepoint_to_string(const system_clock::time_point &tp);

#endif  // REMWHAREAD_TIME_HPP
