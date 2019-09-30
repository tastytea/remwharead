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

#include "export/simple.hpp"
#include "sqlite.hpp"
#include "time.hpp"
#include <string>

namespace remwharead
{
using std::string;

void Export::Simple::print() const
{
    for (const Database::entry & entry : _entries)
    {
        const string timestring = timepoint_to_string(entry.datetime);
        _out << timestring.substr(0, timestring.find('T')) << ": ";
        if (!entry.title.empty())
        {
            _out << entry.title << '\n';
            _out << "            ";
        }

        _out << "<" << entry.uri << ">\n";
    }
}
} // namespace remwharead
