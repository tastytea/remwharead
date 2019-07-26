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

namespace Export
{
    class ExportBase
    {
    public:
        explicit ExportBase(const vector<Database::entry> &entries,
                            ostream &out = cout);

        virtual void print() const = 0;

    protected:
        const vector<Database::entry> _entries;
        ostream &_out;

        //! Sort entries from newest to oldest.
        const vector<Database::entry>
        sort_entries(vector<Database::entry> entries) const;
    };
}

#endif  // REMWHAREAD_EXPORT_HPP
