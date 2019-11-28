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

#include "export/rofi.hpp"
#include "sqlite.hpp"
#include <string>

namespace remwharead
{
using std::string;

void Export::Rofi::print() const
{
    _out << static_cast<char>(0x00) << "markup-rows"
         << static_cast<char>(0x1f) << "true\n";

    for (const Database::entry & entry : _entries)
    {
        _out << entry.title
             << R"( <span size="small" weight="light" style="italic">()"
             << Database::tags_to_string(entry.tags) << ")</span> "
             << R"(<span size="xx-small" weight="ultralight">)"
             << entry.uri << "</span>\n";
    }
}
} // namespace remwharead
