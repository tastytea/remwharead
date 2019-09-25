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

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include "time.hpp"
#include "export/json.hpp"

namespace remwharead
{
    using std::cerr;
    using std::endl;

    void Export::JSON::print() const
    {
        try
        {
            Poco::JSON::Array root = Poco::JSON::Array();

            for (const Database::entry &entry : _entries)
            {
                Poco::JSON::Object json_entry = Poco::JSON::Object();

                json_entry.set("uri", entry.uri);
                json_entry.set("archive_uri", entry.archive_uri);
                json_entry.set("datetime", timepoint_to_string(entry.datetime));
                Poco::JSON::Array tags = Poco::JSON::Array();
                for (const string &tag : entry.tags)
                {
                    tags.add(tag);
                }
                json_entry.set("tags", tags);
                json_entry.set("title", entry.title);
                json_entry.set("description", entry.description);
                json_entry.set("fulltext", entry.fulltext);

                root.add(json_entry);
            }

            root.stringify(_out);
            _out << endl;
        }
        catch (std::exception &e)
        {
            cerr << "Error in " << __func__ << ": " << e.what() << endl;
        }
    }
} // namespace remwharead
