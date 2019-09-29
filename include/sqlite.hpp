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
#include <list>
#include <Poco/Data/Session.h>

namespace remwharead
{
    namespace fs = std::experimental::filesystem;
    using std::string;
    using std::vector;
    using std::chrono::system_clock;
    using time_point = system_clock::time_point;
    using std::list;
    using Poco::Data::Session;

    /*!
     *  @brief  Store and retrieve files from/to SQLite.
     *
     *  @since  0.6.0
     *
     *  @headerfile sqlite.hpp remwharead/sqlite.hpp
     */
    class Database
    {
    public:
        /*!
         *  @brief  Describes a database entry.
         *
         *  @since  0.6.0
         *
         *  @headerfile sqlite.hpp remwharead/sqlite.hpp
         */
        struct entry
        {
            string uri;
            string archive_uri;
            time_point datetime;
            vector<string> tags;
            string title;
            string description;
            string fulltext;

            /*!
             *  @brief  Returns true if date and time are equal.
             *
             *  @since  0.6.0
             */
            friend bool operator ==(const Database::entry &a,
                                    const Database::entry &b);

            /*!
             *  @brief  The full text in one line.
             *
             *  @since  0.6.0
             */
            string fulltext_oneline() const;
        };

        /*!
         *  @brief  Connects to the database and creates it if necessary.
         *
         *  @since  0.6.0
         */
        Database();

        /*!
         *  @brief  Returns true if connected to the database.
         *
         *  @since  0.6.0
         */
        explicit operator bool() const;

        /*!
         *  @brief  Store a Database::entry in the database.
         *
         *  @since  0.6.0
         */
        void store(const entry &data) const;

        /*!
         *  @brief  Retrieve a list of Database::entry from the database.
         *
         *  @since  0.6.0
         */
        list<entry> retrieve(const time_point &start = time_point(),
                             const time_point &end = system_clock::now()) const;

    private:
        fs::path _dbpath;
        std::unique_ptr<Session> _session;
        bool _connected;
    };

    using DB = Database;
} // namespace remwharead

#endif  // REMWHAREAD_SQLITE_HPP
