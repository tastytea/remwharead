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
#include <sqlite/connection.hpp>

namespace fs = std::experimental::filesystem;
using std::string;

class Database
{
public:
    Database();
    operator bool() const;

    void store(const string &uri, const string &archive_uri,
               const string &datetime, const string &tags, const string &title,
               const string &description, const string &fulltext);

private:
    fs::path _dbpath;
    std::unique_ptr<sqlite::connection> _con;
    bool _connected;
};

#endif  // REMWHAREAD_SQLITE_HPP
