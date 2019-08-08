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
#include <thread>
#include <algorithm>
#include <iterator>
#include <list>
#include <cerrno>
#include "sqlite.hpp"
#include "remwharead_cli.hpp"
#include "uri.hpp"
#include "types.hpp"
#include "export/csv.hpp"
#include "export/adoc.hpp"
#include "export/bookmarks.hpp"
#include "export/simple.hpp"
#include "search.hpp"

using namespace remwharead;
using std::cerr;
using std::endl;
using std::string;
using std::chrono::system_clock;
using std::ofstream;
using std::thread;
using std::move;
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
            return EINVAL;
        }
        if (args.size() > 0)
        {
            _uri = args[0];
        }
        if (_uri.empty() && _format == export_format::undefined)
        {
            cerr << "Error: You have to specify either an URI or --export.\n";
            return EINVAL;
        }
    }

    Database db;
    if (!db)
    {
        cerr << "Error: Database could not be opened.\n";
        return EIO;
    }

    if (!_uri.empty())
    {
        URI uri(_uri);
        html_extract page = uri.get();
        if (!page)
        {
            cerr << "Error: Could not fetch page.\n";
            cerr << page.error << endl;
            return EHOSTUNREACH;
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
            return EIO;
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
            const size_t len = entries.size();
            constexpr size_t min_len = 100;
            constexpr size_t min_per_thread = 50;
            const size_t n_threads = thread::hardware_concurrency() / 3 + 1;
            size_t cut_at = len;
            if (len > min_len)
            {   // If there are over `min_len` entries, use `n_threads` threads.
                cut_at = len / n_threads;

                // But don't use less than `min_per_thread` entries per thread.
                if (cut_at < min_per_thread)
                {
                    cut_at = min_per_thread;
                }
            }

            list<list<Database::entry>> segments;

            // Use threads if list is big.
            while (entries.size() > cut_at)
            {
                list<Database::entry> segment;

                auto it = entries.begin();
                std::advance(it, cut_at);

                // Move the first `cut_at` entries into `segments`.
                segment.splice(segment.begin(), entries, entries.begin(), it);
                segments.push_back(move(segment));
            }
            // Move rest of `entries` into `segments`.
            segments.push_back(move(entries));

            list<thread> threads;
            for (auto &segment : segments)
            {
                thread t(
                    [&]
                    {
                        Search search(segment);
                        // Replace `segment` with `result`.
                        segment = search.search_all(_search_all, _regex);
                    });
                threads.push_back(move(t));
            }

            for (thread &t : threads)
            {
                t.join();
                // Move each of `segments` into `entries`.
                entries.splice(entries.end(), segments.front());
                segments.pop_front();
            }
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
        default:
        {
            break;
        }
        }
    }

    return 0;
}

POCO_APP_MAIN(App)
