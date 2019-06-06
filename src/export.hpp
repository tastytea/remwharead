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

#ifndef REMWHAREAD_EXPORT_HPP
#define REMWHAREAD_EXPORT_HPP

#include <vector>
#include <iostream>
#include <map>
#include "time.hpp"
#include "sqlite.hpp"

using std::vector;
using std::ostream;
using std::cout;

namespace Export
{
    class ExportBase
    {
    public:
        explicit ExportBase(const vector<Database::entry> &entries,
                            ostream &out = cout);

    protected:
        const vector<Database::entry> &_entries;
        ostream &_out;
    };

    class CSV : protected ExportBase
    {
    public:
        using ExportBase::ExportBase;

        void print() const;

    private:
        //! replaces " with "".
        const string quote(string field) const;
    };

    class AsciiDoc : protected ExportBase
    {
    public:
        using ExportBase::ExportBase;

        void print() const;

    private:
        using tagmap = std::map<string,vector<Database::entry>>;
        using replacemap = const std::map<const string, const string>;

        const string replace(string text, const replacemap &replacements) const;
        //! Replaces characters in tags that asciidoctor doesn't like.
        const string replace_in_tag(const string &text) const;
        //! Replaces characters in title that asciidoctor doesn't like.
        const string replace_in_title(const string &text) const;
        void print_tags(const tagmap &tags) const;
        const string get_day(const Database::entry &entry) const;
        const string get_time(const Database::entry &entry) const;
    };
}



//! Export as Netscape bookmark file.
void export_bookmarks(const vector<Database::entry> &entries,
                      ostream &out = cout);

#endif  // REMWHAREAD_EXPORT_HPP
