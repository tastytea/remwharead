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

//! @file

namespace remwharead
{
    using std::vector;
    using std::string;

    /*!
     *  @brief   Split expression in subexpressions.
     *
     *  First it splits at `OR` or `||`, then it splits the subexpressions at
     *  `AND` or `&&`. The first vector contains all tags before the first `OR`.
     *
     *  @return  Vector of `OR`-vectors of `AND`-tags.
     */
    const vector<vector<string>> parse_expression(string expression);

    //! Convert str to lowercase. Works with unicode.
    const string to_lowercase(const string &str);

    /*!
     *  @brief   Search in tags of database entries.
     *
     *  Only matches whole tags, *Pill* does not match *Pillow*.
     *
     *  @param   entries    Vector of Database::entry to search.
     *  @param   expression Search expression.
     *  @param   is_re      Is it a regular expression?
     *
     *  @return  Vector of matching Database::entry.
     */
    const vector<Database::entry>
    search_tags(const vector<Database::entry> &entries, string expression,
                const bool is_re);

    /*!
     *  @brief   Search in full text of database entries.
     *
     *  Searches in tags, title, description and full text.
     *
     *  @param   entries    Vector of Database::entry to search.
     *  @param   expression Search expression.
     *  @param   is_re      Is it a regular expression?
     *
     *  @return  Vector of matching Database::entry.
     */
    const vector<Database::entry>
    search_all(const vector<Database::entry> &entries, string expression,
               const bool is_re);
}

#endif  // REMWHAREAD_SEARCH_HPP
