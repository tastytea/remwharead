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

#ifndef REMWHAREAD_SEARCH_HPP
#define REMWHAREAD_SEARCH_HPP

#include <vector>
#include <string>
#include "sqlite.hpp"

using std::vector;
using std::string;

const vector<vector<string>> parse_expression(string expression);
const string to_lowercase(const string &str);

//! Seach database entries for tags.
const vector<Database::entry>
search_tags(const vector<Database::entry> &entries, string expression);

//! Search tags, title, description and full text.
const vector<Database::entry>
search_all(const vector<Database::entry> &entries, string expression);

#endif  // REMWHAREAD_SEARCH_HPP
