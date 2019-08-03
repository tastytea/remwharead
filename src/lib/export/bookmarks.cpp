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

#include <chrono>
#include <string>
#include "sqlite.hpp"
#include "export/bookmarks.hpp"

namespace remwharead
{
    using std::chrono::system_clock;
    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::string;

    void Export::Bookmarks::print() const
    {
        _out << "<!DOCTYPE NETSCAPE-Bookmark-file-1>\n"
            "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; "
            "charset=UTF-8\">\n"
            "<TITLE>Bookmarks from remwharead</TITLE>\n"
            "<H1>Bookmarks from remwharead<H1>\n\n"
            "<DL><p>\n"
            "<DT><H3>remwharead</H3>\n"
            "<DL><p>\n";

        for (const Database::entry & entry : _entries)
        {
            string title = entry.title;
            if (title.empty())
            {
                title = entry.uri;
            }
            system_clock::time_point tp = entry.datetime;
            system_clock::duration duration = tp.time_since_epoch();
            string time_seconds =
                std::to_string(duration_cast<seconds>(duration).count());

            _out << "<DT><A HREF=\"" << entry.uri << "\" "
                 << "ADD_DATE=\"" << time_seconds << "\">"
                 << title << "</A>\n";
        }
        _out << "</DL><p>\n"
             << "</DL><p>\n";
    }
}
