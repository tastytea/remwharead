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

#ifndef REMWHAREAD_EXPORT_HPP
#define REMWHAREAD_EXPORT_HPP

#include <vector>
#include <iostream>
#include "sqlite.hpp"

using std::vector;
using std::ostream;
using std::cout;

void export_csv(const vector<Database::entry> &entries, ostream &out = cout);
//! replaces " with "".
const string quote_csv(string field);

void export_adoc(const vector<Database::entry> &entries, ostream &out = cout);
//! Replaces characters in tags that asciidoctor doesn't like.
const string replace_in_tags(string text);


//! Export as Netscape bookmark file.
void export_bookmarks(const vector<Database::entry> &entries,
                      ostream &out = cout);
#endif  // REMWHAREAD_EXPORT_HPP
