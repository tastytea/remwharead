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
#include <exception>
#include <popl.hpp>
#include "version.hpp"
#include "parse_options.hpp"

using std::cout;
using std::cerr;
using std::endl;

options::options()
{}

options::options(const uint8_t &status)
    : status_code(status)
{}

const options parse_options(const int argc, const char *argv[])
{
    string tags;
    string format;
    string span;
    options opts;

    try
    {
        popl::OptionParser op("Available options");
        op.add<popl::Value<string>>
            ("t", "tags", "Add tags to URI, delimited by commas.", "", &tags);
        auto option_export = op.add<popl::Implicit<string>>
            ("e", "export", "Export to format.", "simple", &format);
        op.add<popl::Value<string>>
            ("f", "file", "Save output to file.", "", &opts.file);
        op.add<popl::Value<string>>
            ("T", "time-span",
             "Only export entries between YYYY-MM-DD,YYYY-MM-DD.", "", &span);
        op.add<popl::Value<string>>
            ("s", "search-tags",
             "Search in tags. Format: tag1 AND tag2 OR tag3.",
             "", &opts.search_tags);
        op.add<popl::Value<string>>
            ("S", "search-all",
             "Search in tags, title, description and full text.",
             "", &opts.search_all);
        auto option_noarchive = op.add<popl::Switch>
            ("N", "no-archive", "Do not archive URI.");
        auto option_help = op.add<popl::Switch>
            ("h", "help", "Show this help message.");
        auto option_version = op.add<popl::Switch>
            ("V", "version", "Print version, copyright and license.");
        op.parse(argc, argv);

        if (option_help->is_set())
        {
            cout << "Usage: " << argv[0] << " [-t tags] [-N] URI\n"
                 << "       " << argv[0]
                 << " -e format [-f file] [-T start,end] "
                 << "[[-s|-S] expression]\n";
            cout << op;
            return options(0);
        }

        if (option_version->is_set())
        {
            cout << "remwharead " << global::version << endl <<
    "Copyright (C) 2019 tastytea <tastytea@tastytea.de>\n"
    "License GPLv3: GNU GPL version 3 "
    "<https://www.gnu.org/licenses/gpl-3.0.html>.\n"
    "This program comes with ABSOLUTELY NO WARRANTY. This is free software,\n"
    "and you are welcome to redistribute it under certain conditions.\n";
            return options(0);
        }

        if (option_noarchive->is_set())
        {
            opts.archive = false;
        }

        if (!tags.empty())
        {
            size_t pos_end = 0;
            size_t pos_start = 0;
            while (pos_end != std::string::npos)
            {
                pos_end = tags.find(',', pos_start);
                string buffer = tags.substr(pos_start, pos_end - pos_start);
                while (*buffer.begin() == ' ') // Remove leading spaces.
                {
                    buffer.erase(buffer.begin());
                }
                while (*buffer.rbegin() == ' ') // Remove trailing spaces.
                {
                    buffer.erase(buffer.end() - 1);
                }
                opts.tags.push_back(buffer);
                pos_start = pos_end + 1;
            }
        }

        if (option_export->is_set())
        {
            if (format == "csv")
            {
                opts.format = export_format::csv;
            }
            else if (format == "asciidoc" || format == "adoc")
            {
                opts.format = export_format::asciidoc;
            }
            else if (format == "bookmarks")
            {
                opts.format = export_format::bookmarks;
            }
            else if (format == "simple")
            {
                opts.format = export_format::simple;
            }
            else
            {
                opts.format = export_format::undefined;
                cerr << "Error: Export format must be "
                     << "csv, asciidoc, bookmarks or simple.\n";
                return options(1);
            }
        }

        if (!span.empty())
        {
            size_t pos = span.find(',');
            if (pos != std::string::npos)
            {
                opts.span =
                    {
                        string_to_timepoint(span.substr(0, pos)),
                        string_to_timepoint(span.substr(pos + 1))
                    };
            }
            else
            {
                cerr << "Error: Time span must be in format: "
                    "YYYY-MM-DD,YYYY-MM-DD.\n";
                return options(1);
            }
        }

        if (op.non_option_args().size() > 0)
        {
            opts.uri = op.non_option_args().front();
        }

        if (opts.uri == "" && opts.format == export_format::undefined)
        {
            cerr << "Error: You have to specify either URI or --export.\n";
            return options(1);
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
        return options(1);
    }

    return opts;
}
