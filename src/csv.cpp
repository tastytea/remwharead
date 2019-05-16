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

#include <iostream>
#include <regex>
#include "time.hpp"
#include "csv.hpp"

using std::cerr;
using std::endl;
using std::regex;
using std::regex_replace;

void export_csv(const vector<Database::entry> &entries, ostream &out)
{
    try
    {
        out << "\"URI\",\"Archived URI\",\"Date & time\",\"Tags\","
            << "\"Title\",\"Description\",\"Full text\"\r\n";
        for (const Database::entry &entry : entries)
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
            out << '"' << quote_csv(entry.uri) << "\",\""
                << quote_csv(entry.archive_uri) << "\",\""
                << timepoint_to_string(entry.datetime) << "\",\""
                << quote_csv(strtags) << "\",\""
                << quote_csv(entry.title) << "\",\""
                << quote_csv(entry.description) << "\",\""
                << quote_csv(entry.fulltext_oneline()) << '"'<< "\r\n";
        }
    }
    catch (std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
    }
}

const string quote_csv(const string &field)
{
    return regex_replace(field, regex("\""), "\"\"");
}
