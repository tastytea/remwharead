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

#include "export/csv.hpp"
#include "time.hpp"

namespace remwharead
{
using std::cerr;
using std::endl;

void Export::CSV::print() const
{
    try
    {
        _out << R"("URI","Archived URI","Date & time","Tags",)"
             << R"("Title","Description","Full text")" << "\r\n";
        for (const Database::entry &entry : _entries)
        {
            string strtags;
            for (const string &tag : entry.tags)
            {
                strtags += tag;
                if (tag != *(entry.tags.rbegin()))
                {
                    strtags += ",";
                }
            }
            _out << '"' << quote(entry.uri) << "\",\""
                 << quote(entry.archive_uri) << "\",\""
                 << timepoint_to_string(entry.datetime) << "\",\""
                 << quote(strtags) << "\",\""
                 << quote(entry.title) << "\",\""
                 << quote(entry.description) << "\",\""
                 << quote(entry.fulltext_oneline()) << '"'<< "\r\n";
        }
    }
    catch (std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
    }
}

string Export::CSV::quote(string field) const
{
    size_t pos = 0;
    while ((pos = field.find('"', pos)) != std::string::npos)
    {
        field.replace(pos, 1, "\"\"");
    }
    return field;
}
} // namespace remwharead
