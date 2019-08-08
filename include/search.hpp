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

#include <list>
#include <string>
#include <vector>
#include "sqlite.hpp"

namespace remwharead
{
    using std::list;
    using std::string;
    using std::vector;

    /*!
     *  @brief  Search in database entries.
     *
     *  @since  0.7.0
     *
     *  @headerfile search.hpp remwharead/search.hpp
     */
    class Search
    {
    public:
        /*!
         *  @brief  Defines the entries to search.
         *
         *  @since  0.7.0
         */
        explicit Search(const list<Database::entry> &entries);

        /*!
         *  @brief  Search in tags of database entries.
         *
         *  Only matches whole tags, *Pill* does not match *Pillow*.
         *
         *  @param  expression Search expression.
         *  @param  is_re      Is it a regular expression?
         *
         *  @return Vector of matching Database::entry.
         *
         *  @since  0.7.0
         */
        const list<Database::entry> search_tags(string expression,
                                                        const bool is_re) const;

        /*!
     *  @brief  Search in full text of database entries.
     *
     *  Searches in tags, title, description and full text.
     *
     *  @param  entries    Vector of Database::entry to search.
     *  @param  expression Search expression.
     *  @param  is_re      Is it a regular expression?
     *
     *  @return Vector of matching Database::entry.
     *
     *  @since  0.7.0
     */
    const list<Database::entry> search_all(string expression,
                                                   const bool is_re) const;

    private:
        const list<Database::entry> _entries;

        /*!
         *  @brief  Split expression into subexpressions.
         *
         *  First it splits at `OR` or `||`, then it splits the subexpressions
         *  at `AND` or `&&`. The first vector contains all tags before the
         *  first `OR`.
         *
         *  @return Vector of `OR`-vectors of `AND`-tags.
         *
         *  @since  0.7.0
         */
        const vector<vector<string>> parse_expression(string expression) const;

        /*!
         *  @brief  Convert str to lowercase. Works with unicode.
         *
         *  @since  0.7.0
         */
        inline const string to_lowercase(const string &str) const;
    };
}

#endif  // REMWHAREAD_SEARCH_HPP
