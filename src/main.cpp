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
#include "sqlite.hpp"
#include "parse_options.hpp"
#include "uri.hpp"
#include "export.hpp"
#include "search.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::chrono::system_clock;

int main(const int argc, const char *argv[])
{
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
            cerr << "Error: Could not fetch page.";
            return 4;
        }
        db.store({opts.uri, uri.archive(), system_clock::now(), opts.tags,
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
        vector<Database::entry> entries =
            db.retrieve(opts.span[0], opts.span[1]);
        if (!opts.search_tags.empty())
        {
            entries = search_tags(entries, opts.search_tags);
        }

        switch (opts.format)
        {
        case export_format::csv:
        {
            if (file.is_open())
            {
                export_csv(entries, file);
                file.close();
            }
            else
            {
                export_csv(entries);
            }
            break;
        }
        case export_format::asciidoc:
        {
            if (file.is_open())
            {
                export_adoc(entries, file);
                file.close();
            }
            else
            {
                export_adoc(entries);
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
