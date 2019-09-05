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
#include <locale>
#include <list>
#include "sqlite.hpp"
#include "remwharead_cli.hpp"
#include "uri.hpp"
#include "types.hpp"
#include "export/csv.hpp"
#include "export/adoc.hpp"
#include "export/bookmarks.hpp"
#include "export/simple.hpp"
#include "export/json.hpp"
#include "export/rss.hpp"
#include "search.hpp"

using namespace remwharead;
using std::cerr;
using std::endl;
using std::string;
using std::chrono::system_clock;
using std::ofstream;
using std::list;

int App::main(const std::vector<std::string> &args)
{
    std::locale::global(std::locale("")); // Set locale globally.

    if (_version_requested)
    {
        print_version();
    }
    else if (_help_requested)
    {
        print_help();
    }
    else
    {
        if (_argument_error)
        {
            return 1;
        }
        if (args.size() > 0)
        {
            _uri = args[0];
        }
        if (_uri.empty() && _format == export_format::undefined)
        {
            cerr << "Error: You have to specify either an URI or --export.\n";
            return 1;
        }
    }

    Database db;
    if (!db)
    {
        cerr << "Error: Database could not be opened.\n";
        return 2;
    }

    if (!_uri.empty())
    {
        URI uri(_uri);
        html_extract page = uri.get();
        if (!page)
        {
            cerr << "Error: Could not fetch page.\n";
            cerr << page.error << endl;
            return 3;
        }
        archive_answer archive;
        if (_archive)
        {
            archive = uri.archive();
            if (!archive)
            {
                cerr << "Error archiving URL: " << archive.error << endl;
            }
        }
        db.store({_uri, archive.uri, system_clock::now(), _tags,
                  page.title, page.description, page.fulltext});
    }

    ofstream file;
    if (!_file.empty())
    {
        file.open(_file);
        if (!file.good())
        {
            cerr << "Error: Could not open file: " << _file << endl;
            return 2;
        }
    }

    if (_format != export_format::undefined)
    {
        list<Database::entry> entries = db.retrieve(_timespan[0], _timespan[1]);

        if (!_search_tags.empty())
        {
            Search search(entries);
            entries = search.search_tags(_search_tags, _regex);
        }
        else if (!_search_all.empty())
        {
            Search search(entries);
            entries = search.search_all_threaded(_search_all, _regex);
        }

        switch (_format)
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
        case export_format::json:
        {
            if (file.is_open())
            {
                Export::JSON(entries, file).print();
                file.close();
            }
            else
            {
                Export::JSON(entries).print();
            }
            break;
        }
        case export_format::rss:
        {
            if (file.is_open())
            {
                Export::RSS(entries, file).print();
                file.close();
            }
            else
            {
                Export::RSS(entries).print();
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

POCO_APP_MAIN(App)
