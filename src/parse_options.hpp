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

#ifndef REMWHAREAD_PARSE_OPTIONS_HPP
#define REMWHAREAD_PARSE_OPTIONS_HPP

#include <string>
#include <vector>
#include <array>
#include <chrono>
#include <cstdint>

using std::string;
using std::vector;
using std::array;
using std::chrono::system_clock;
using std::uint8_t;

enum class export_format
{
    undefined,
    csv,
    asciidoc
};

typedef struct options
{
    vector<string> tags;
    export_format format = export_format::undefined;
    string file;
    array<system_clock::time_point, 2> span;
    string url;
    uint8_t status_code = 0;

    options();
    explicit options(const uint8_t &status);
} options;

// Convert ISO 8601 time-string to time_point.
const system_clock::time_point string_to_timepoint(const string &strtime);

// Parse command-line options.
const options parse_options(const int argc, const char *argv[]);

#endif  // REMWHAREAD_PARSE_OPTIONS_HPP
