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

#include "remwharead_cli.hpp"
#include "sqlite.hpp"
#include "time.hpp"
#include "version.hpp"
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <iostream>
#include <memory>

using namespace remwharead_cli;
using std::cout;
using std::cerr;
using std::endl;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

App::App()
    : _exit_requested{false}
    , _argument_error{false}
    , _format{export_format::undefined}
    , _timespan{{time_point(), system_clock::now()}}
    , _archive{true}
    , _regex{false}
{}

void App::defineOptions(OptionSet& options)
{
    options.addOption(
        Option("help", "h", "Show this help message.")
        .argument("option", false)
        .callback(OptionCallback<App>(this, &App::handle_options)));
    options.addOption(
        Option("version", "V", "Print version, copyright and license.")
        .callback(OptionCallback<App>(this, &App::handle_options)));
    options.addOption(
        Option("tags", "t", "Add tags to URI, delimited by commas.")
        .argument("tags")
        .callback(OptionCallback<App>(this, &App::handle_options)));
    options.addOption(
        Option("export", "e", "Export to format. See manpage for a list.")
        .argument("format")
        .callback(OptionCallback<App>(this, &App::handle_options)));
    options.addOption(
        Option("file", "f", "Save output to file.")
        .argument("file")
        .callback(OptionCallback<App>(this, &App::handle_options)));
    options.addOption(
        Option("time-span", "T",
               "Only export entries between YYYY-MM-DD,YYYY-MM-DD.")
        .argument("times")
        .callback(OptionCallback<App>(this, &App::handle_options)));
    options.addOption(
        Option("search-tags", "s",
               "Search in tags. Format: tag1 AND tag2 OR tag3.")
        .argument("expression")
        .callback(OptionCallback<App>(this, &App::handle_options)));
    options.addOption(
        Option("search-all", "S",
               "Search in tags, title, description and full text.")
        .argument("expression")
        .callback(OptionCallback<App>(this, &App::handle_options)));
    options.addOption(
        Option("regex", "r", "Use regular expression for search.")
        .callback(OptionCallback<App>(this, &App::handle_options)));
    options.addOption(
        Option("no-archive", "N", "Do not archive URI.")
        .callback(OptionCallback<App>(this, &App::handle_options)));
    options.addOption(
        Option("delete", "d",
               "Remove all entries with this URI from database.")
        .argument("URI")
        .callback(OptionCallback<App>(this, &App::handle_options)));
}

void App::handle_options(const std::string &name, const std::string &value)
{
    if (name == "help")
    {
        _exit_requested = true;
        print_help(value);
        stopOptionsProcessing();
    }
    else if (name == "version")
    {
        _exit_requested = true;
        print_version();
        stopOptionsProcessing();
    }
    else if (name == "tags")
    {
        size_t pos_end = 0;
        size_t pos_start = 0;
        while (pos_end != std::string::npos)
        {
            pos_end = value.find(',', pos_start);
            string buffer = value.substr(pos_start, pos_end - pos_start);
            while (*buffer.begin() == ' ') // Remove leading spaces.
            {
                buffer.erase(buffer.begin());
            }
            while (*buffer.rbegin() == ' ') // Remove trailing spaces.
            {
                buffer.erase(buffer.end() - 1);
            }
            if (!buffer.empty())
            {
                _tags.push_back(buffer);
            }
            pos_start = pos_end + 1;
        }
    }
    else if (name == "export")
    {
        if (value == "csv")
        {
            _format = export_format::csv;
        }
        else if (value == "asciidoc" || value == "adoc")
        {
            _format = export_format::asciidoc;
        }
        else if (value == "bookmarks")
        {
            _format = export_format::bookmarks;
        }
        else if (value == "simple")
        {
            _format = export_format::simple;
        }
        else if (value == "json")
        {
            _format = export_format::json;
        }
        else if (value == "rss")
        {
            _format = export_format::rss;
        }
        else if (value == "link")
        {
            _format = export_format::link;
        }
        else if (value == "rofi")
        {
            _format = export_format::rofi;
        }
        else
        {
            cerr << "Error: Unknown format.\n";
            _argument_error = true;
        }
    }
    else if (name == "file")
    {
        _file = value;
    }
    else if (name == "time-span")
    {
        size_t pos = value.find(',');
        if (pos != std::string::npos)
        {
            _timespan =
                {
                    string_to_timepoint(value.substr(0, pos)),
                    string_to_timepoint(value.substr(pos + 1))
                };
        }
        else
        {
            cerr << "Error: Time span must be in format: "
                "YYYY-MM-DD,YYYY-MM-DD.\n";
            _argument_error = true;
        }
    }
    else if (name == "search-tags")
    {
        _search_tags = value;
    }
    else if (name == "search-all")
    {
        _search_all = value;
    }
    else if (name == "no-archive")
    {
        _archive = false;
    }
    else if (name == "regex")
    {
        _regex = true;
    }
    else if (name == "delete")
    {
        Database db;
        cout << "Deleted " << db.remove(value) << " entries.\n";
        _exit_requested = true;
    }
}

void App::print_help(const string &option)
{
    std::unique_ptr<HelpFormatter> helpFormatter;
    OptionSet oneoption;

    if (option.empty())
    {
        helpFormatter = std::make_unique<HelpFormatter>(options());
        helpFormatter->setCommand(commandName());
        helpFormatter->setUsage("[-t tags] [-N] URI\n"
                                "-e format [-f file] [-T start,end] "
                                "[[-s|-S] expression] [-r]\n"
                                "-d URI");
    }
    else
    {
        oneoption.addOption(options().getOption(option));
        helpFormatter = std::make_unique<HelpFormatter>(oneoption);
    }

    helpFormatter->format(cout);
}

void App::print_version()
{
    cout << "remwharead " << remwharead::version << endl <<
        "Copyright (C) 2019 tastytea <tastytea@tastytea.de>\n"
        "License GPLv3: GNU GPL version 3 "
        "<https://www.gnu.org/licenses/gpl-3.0.html>.\n"
        "This program comes with ABSOLUTELY NO WARRANTY. This is free software,"
        "\nand you are welcome to redistribute it under certain conditions.\n";
}
