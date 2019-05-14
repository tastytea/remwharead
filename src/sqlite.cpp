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
#include <basedir.h>
#include <sqlite/execute.hpp>
#include <sqlite/query.hpp>
#include "time.hpp"
#include "sqlite.hpp"

using std::cerr;
using std::endl;

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
        cerr << "Error: " << e.what() << endl;
    }
}

Database::operator bool() const
{
    return _connected;
}

void Database::store(const string &uri, const string &archive_uri,
                     const system_clock::time_point &datetime,
                     const vector<string> &tags, const string &title,
                     const string &description, const string &fulltext)
{
    try
    {
        const string strdatetime = timepoint_to_string(datetime);
        string strtags;
        for (const string &tag : tags)
        {
            strtags += tag;
            if (tag != *(tags.rbegin()))
            {
                strtags += ",";
            }
        }

        sqlite::execute ins(*_con, "INSERT INTO remwharead "
                            "VALUES(?, ?, ?, ?, ?, ?, ?);");
        ins % uri % archive_uri % strdatetime % strtags
            % title % description % fulltext;
        ins();
    }
    catch (std::exception &e)
    {
        cerr << "Error: " << e.what() << endl;
    }
}
