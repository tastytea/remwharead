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
#include <string>
#include <chrono>
#include <fstream>
#include <memory>
#include <locale>
#include "sqlite.hpp"
#include "parse_options.hpp"
#include "uri.hpp"
#include "types.hpp"
#include "export/csv.hpp"
#include "export/adoc.hpp"
#include "export/bookmarks.hpp"
#include "export/simple.hpp"
#include "search.hpp"

using namespace remwharead;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::chrono::system_clock;

int main(const int argc, const char *argv[])
{
    std::locale::global(std::locale("")); // Set locale globally.

    options opts = parse_options(argc, argv);
    if (opts.status_code != 0)
    {
        return opts.status_code;
    }

    Database db;

    if (!db)
    {
        cerr << "Error: Database connection failed.\n";
        return 2;
    }

    if (!opts.uri.empty())
    {
        URI uri(opts.uri);
        html_extract page = uri.get();
        if (page.fulltext.empty())
        {
            cerr << "Error: Could not fetch page.\n";
            return 4;
        }
        string archive_uri;
        if (opts.archive)
        {
            archive_uri = uri.archive();
        }
        db.store({opts.uri, archive_uri, system_clock::now(), opts.tags,
                  page.title, page.description, page.fulltext});
    }

    std::ofstream file;
    if (!opts.file.empty())
    {
        file.open(opts.file);
        if (!file.good())
        {
            cerr << "Error: Could not open file: " << opts.file << endl;
            return 3;
        }
    }
    if (opts.format != export_format::undefined)
    {
        vector<Database::entry> entries;
        Search search(db.retrieve(opts.span[0], opts.span[1]));

        if (!opts.search_tags.empty())
        {
            entries = search.search_tags(opts.search_tags, opts.regex);
        }
        else if (!opts.search_all.empty())
        {
            entries = search.search_all(opts.search_all, opts.regex);
        }

        switch (opts.format)
        {
        case export_format::csv:
        {
            if (file.is_open())
            {
                Export::CSV(entries, file).print();
                file.close();
            }
            else
            {
                Export::CSV(entries).print();
            }
            break;
        }
        case export_format::asciidoc:
        {
            if (file.is_open())
            {
                Export::AsciiDoc(entries, file).print();
                file.close();
            }
            else
            {
                Export::AsciiDoc(entries).print();
            }
            break;
        }
        case export_format::bookmarks:
        {
            if (file.is_open())
            {
                Export::Bookmarks(entries, file).print();
                file.close();
            }
            else
            {
                Export::Bookmarks(entries).print();
            }
            break;
        }
        case export_format::simple:
        {
            if (file.is_open())
            {
                Export::Simple(entries, file).print();
                file.close();
            }
            else
            {
                Export::Simple(entries).print();
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }

    return 0;
}
