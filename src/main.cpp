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
#include "sqlite.hpp"
#include "parse_options.hpp"
#include "csv.hpp"
#include "uri.hpp"

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
        db.store({opts.uri, uri.archive(), system_clock::now(), opts.tags,
                  page.title, page.description, page.fulltext});
    }

    switch (opts.format)
    {
    case export_format::csv:
    {
        export_csv(db.retrieve(opts.span[0], opts.span[1]));
        break;
    }
    case export_format::asciidoc:
    {
        cerr << "Error: AsciiDoc is not yet supported.\n";
        break;
    }
    default:
    {
        // Do nothing.
        break;
    }
    }

    return 0;
}
