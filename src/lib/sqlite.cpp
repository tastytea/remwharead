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

#include "sqlite.hpp"
#include "time.hpp"
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/Version.h>
#include <Poco/Environment.h>
#include <algorithm>
#include <exception>
#include <iostream>

namespace remwharead
{
using std::cerr;
using std::endl;
using namespace Poco::Data::Keywords;
using Poco::Data::Statement;
using Poco::Environment;

Database::Database()
    : _connected(false)
{
    try
    {
        _dbpath = get_data_home();
        if (!fs::exists(_dbpath))
        {
            fs::create_directories(_dbpath);
        }
        _dbpath /= "database.sqlite";

        Poco::Data::SQLite::Connector::registerConnector();
        _session = std::make_unique<Session>("SQLite", _dbpath);
        *_session << "CREATE TABLE IF NOT EXISTS remwharead("
            "uri TEXT, archive_uri TEXT, datetime TEXT, "
            "tags TEXT, title TEXT, description TEXT, fulltext TEXT);", now;

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

bool operator ==(const Database::entry &a, const Database::entry &b)
{
    return (a.datetime == b.datetime);
}

string Database::entry::fulltext_oneline() const
{
    string oneline = fulltext;
    size_t pos = 0;
    while ((pos = oneline.find('\n', pos)) != string::npos)
    {
        oneline.replace(pos, 1, "\\n");
    }
    return oneline;
}

void Database::store(const Database::entry &data) const
{
    try
    {
        const string strdatetime = timepoint_to_string(data.datetime, true);
        string strtags;
        Statement insert(*_session);

        for (const string &tag : data.tags)
        {
            strtags += tag;
            if (tag != *(data.tags.rbegin()))
            {
                strtags += ",";
            }
        }

        // useRef() uses the const reference.
        insert << "INSERT INTO remwharead "
            "VALUES(?, ?, ?, ?, ?, ?, ?);",
            useRef(data.uri), useRef(data.archive_uri),
            useRef(strdatetime), useRef(strtags), useRef(data.title),
            useRef(data.description), useRef(data.fulltext);
        insert.execute();
    }
    catch (std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
    }
}

list<Database::entry> Database::retrieve(const time_point &start,
                                         const time_point &end) const
{
    try
    {
        Database::entry entrybuf;
        string datetime;
        string strtags;
        Statement select(*_session);

        // bind() copies the value.
        select << "SELECT * FROM remwharead WHERE datetime "
            "BETWEEN ? AND ? ORDER BY datetime DESC;",
            bind(timepoint_to_string(start, true)),
            bind(timepoint_to_string(end, true)),
            into(entrybuf.uri), into(entrybuf.archive_uri), into(datetime),
            into(strtags), into(entrybuf.title), into(entrybuf.description),
            into(entrybuf.fulltext), range(0, 1);

        list<entry> entries;

        while(!select.done() && select.execute() != 0)
        {
            entrybuf.datetime = string_to_timepoint(datetime, true);

            vector<string> tags;
            size_t pos = 0;
            while (pos != string::npos)
            {
                const size_t newpos = strtags.find(',', pos);
                const string tag = strtags.substr(pos, newpos - pos);
                if (!tag.empty())
                {
                    tags.push_back(tag);
                }
                pos = newpos;
                if (pos != string::npos)
                {
                    ++pos;
                }
            }
            entrybuf.tags = tags;

            entries.push_back(entrybuf);
        }

        return entries;
    }
    catch (std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
    }

    return {};
}

size_t Database::remove(const string &uri)
{
    Statement del(*_session);

    del << "DELETE FROM remwharead WHERE uri = ?;", bind(uri);

    return del.execute();
}

fs::path Database::get_data_home() const
{
    fs::path path;

    if (Environment::has("XDG_DATA_HOME"))
    {
        path = Environment::get("XDG_DATA_HOME") / fs::path("remwharead");
    }
    else if (Environment::has("HOME"))
    {
        path = Environment::get("HOME") / fs::path(".local/share/remwharead");
    } // Else return empty path.

    return path;
}
} // namespace remwharead
