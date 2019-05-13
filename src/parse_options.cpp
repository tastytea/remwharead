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
#include <ctime>
#include <iomanip>
#include <sstream>
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

const system_clock::time_point string_to_timepoint(const string &strtime)
{
    std::stringstream sstime(strtime);
    struct std::tm tm = {};
    tm.tm_isdst = -1;           // Detect daylight saving time.
    sstime >> std::get_time(&tm, "%Y-%m-%dT%T");
    std::time_t time = timelocal(&tm); // Assume time is local.
    return system_clock::from_time_t(time);
}

const options parse_options(const int argc, const char *argv[])
{
    string tags;
    string format;
    string file;
    string span;
    options opts;

    try
    {
        popl::OptionParser op("Available options");
        op.add<popl::Value<string>>
            ("t", "tags", "Add tags to URL, delimited by commas.", "", &tags);
        op.add<popl::Value<string>>
            ("e", "export", "Export to format.", "", &format);
        op.add<popl::Value<string>>
            ("f", "file", "Save output to file.", "", &file);
        op.add<popl::Value<string>>
            ("s", "span", "Only export entries between YYYY-MM-DD,YYYY-MM-DD.",
             "", &span);
        auto option_help = op.add<popl::Switch>
            ("h", "help", "Show this help message.");
        auto option_version = op.add<popl::Switch>
            ("V", "version", "Print version, copyright and license.");
        op.parse(argc, argv);

        if (option_help->is_set())
        {
            cout << "Usage: " << argv[0] << " [-t tags] URL\n"
                 << "       " << argv[0]
                 << " -e format [-f file] [-s start,end]\n";
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

        if (!tags.empty())
        {
            size_t pos_end = 0;
            size_t pos_start = 0;
            while (pos_end != std::string::npos)
            {
                pos_end = tags.find(',', pos_start);
                opts.tags.push_back
                    (tags.substr(pos_start, pos_end - pos_start));
                pos_start = pos_end + 1;
            }
        }

        if (!format.empty())
        {
            if (format == "csv")
            {
                opts.format = export_format::csv;
            }
            else if (format == "asciidoc")
            {
                opts.format = export_format::asciidoc;
            }
            else
            {
                cerr << "Error: Export format must be csv or asciidoc.\n";
                return options(1);
            }
        }

        opts.file = file;

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
    }
    catch (const std::exception &e)
    {
        cerr << "Error: " << e.what() << endl;
        return options(1);
    }

    return opts;
}
