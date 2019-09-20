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

#include <algorithm>
#include "export/export.hpp"

namespace remwharead
{
namespace Export
{
    ExportBase::ExportBase(const list<Database::entry> &entries, ostream &out)
        : _entries(sort_entries(entries))
        , _out(out)
    {}

    const list<Database::entry>
    ExportBase::sort_entries(list<Database::entry> entries) const
    {
        entries.sort([](const auto &a, const auto &b)
                     {
                         return (a.datetime > b.datetime);
                     });
        entries.unique();

        return entries;
    }
}
}
