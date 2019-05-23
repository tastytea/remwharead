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

#ifndef REMWHAREAD_SQLITE_HPP
#define REMWHAREAD_SQLITE_HPP

#include <experimental/filesystem>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <sqlite/connection.hpp>
#include "types.hpp"

namespace fs = std::experimental::filesystem;
using std::string;
using std::vector;
using std::chrono::system_clock;
using time_point = system_clock::time_point;

class Database
{
public:
    typedef struct entry
    {
        string uri;
        string archive_uri;
        time_point datetime;
        vector<string> tags;
        string title;
        string description;
        string fulltext;

        //! Returns true if date & time are equal.
        friend bool operator ==(const Database::entry &a,
                                const Database::entry &b);
        //! The full text in one line.
        const string fulltext_oneline() const;
    } entry;

    Database();
    operator bool() const;

    //! Store in database.
    void store(const entry &data) const;

    //! retrieve from database.
    const vector<entry> retrieve(const time_point &start = time_point(),
                                 const time_point &end = system_clock::now())
        const;

private:
    fs::path _dbpath;
    std::unique_ptr<sqlite::connection> _con;
    bool _connected;
};

#endif  // REMWHAREAD_SQLITE_HPP
