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

namespace remwharead
{
namespace Export
{
    class ExportBase
    {
    public:
        /*!
         *  @brief   Export vector of Database::entry.
         *
         *  @param   entries Vector of Database::entry to export.
         *  @param   out     Output stream.
         */
        explicit ExportBase(const vector<Database::entry> &entries,
                            ostream &out = cout);

        /*!
         *  @brief   Print output to std::ostream.
         */
        virtual void print() const = 0;

    protected:
        const vector<Database::entry> _entries;
        ostream &_out;

        /*!
         *  @brief   Sort entries from newest to oldest.
         *
         *  @param   entries Vector of Database::entry to sort.
         *
         *  @return  Sorted vector of Database::entry.
         */
        const vector<Database::entry>
        sort_entries(vector<Database::entry> entries) const;
    };
}
}

#endif  // REMWHAREAD_EXPORT_HPP
