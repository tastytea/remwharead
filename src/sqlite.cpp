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

#include <exception>
#include <iostream>
#include <algorithm>
#include <regex>
#include <basedir.h>
#include <sqlite/execute.hpp>
#include <sqlite/query.hpp>
#include "time.hpp"
#include "sqlite.hpp"

using std::cerr;
using std::endl;
using std::regex;
using std::regex_replace;

Database::Database()
    : _connected(false)
{
    try
    {
        xdgHandle xdg;
        xdgInitHandle(&xdg);
        _dbpath = xdgDataHome(&xdg) / fs::path("remwharead");
        xdgWipeHandle(&xdg);

        if (!fs::exists(_dbpath))
        {
            fs::create_directories(_dbpath);
        }
        _dbpath /= "database.sqlite";

        _con = std::make_unique<sqlite::connection>(_dbpath);
        sqlite::execute(*_con, "CREATE TABLE IF NOT EXISTS remwharead("
                        "uri TEXT, archive_uri TEXT, datetime TEXT, tags TEXT, "
                        "title TEXT, description TEXT, fulltext TEXT);", true);

        _connected = true;
    }
    catch (std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
    }
}

Database::operator bool() const
{
    return _connected;
}

const string Database::entry::fulltext_oneline() const
{
    return regex_replace(fulltext, regex("\n"), "\\n");
}

void Database::store(const Database::entry &data) const
{
    try
    {
        const string strdatetime = timepoint_to_string(data.datetime, true);
        string strtags;
        for (const string &tag : data.tags)
        {
            strtags += tag;
            if (tag != *(data.tags.rbegin()))
            {
                strtags += ",";
            }
        }

        sqlite::execute ins(*_con, "INSERT INTO remwharead "
                            "VALUES(?, ?, ?, ?, ?, ?, ?);");
        ins % data.uri % data.archive_uri % strdatetime % strtags
            % data.title % data.description % data.fulltext;
        ins();
    }
    catch (std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
    }
}

const vector<Database::entry> Database::retrieve(const time_point &start,
                                                 const time_point &end) const
{
    try
    {
        const string query = "SELECT * FROM remwharead WHERE datetime "
            "BETWEEN '" + timepoint_to_string(start, true)
            +  "' AND '" + timepoint_to_string(end, true)
            + "' ORDER BY datetime;";

        sqlite::query q(*_con, query);
        sqlite::result_type res = q.get_result();
        vector<entry> entries;

        while(res->next_row())
        {
            vector<string> tags;
            const string strtags = res->get_string(3);
            size_t pos = 0;
            while (pos != std::string::npos)
            {
                const size_t newpos = strtags.find(',', pos);
                tags.push_back(strtags.substr(pos, newpos - pos));
                pos = newpos;
                if (pos != std::string::npos)
                {
                    ++pos;
                }
            }
            entries.push_back
                ({
                    res->get_string(0),
                    res->get_string(1),
                    string_to_timepoint(res->get_string(2), true),
                    tags,
                    res->get_string(4),
                    res->get_string(5),
                    res->get_string(6)
                });
        }

        return entries;
    }
    catch (std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
    }

    return {};
}
