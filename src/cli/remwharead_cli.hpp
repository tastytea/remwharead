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

#ifndef REMWHAREAD_PARSE_OPTIONS_HPP
#define REMWHAREAD_PARSE_OPTIONS_HPP

#include "types.hpp"
#include <Poco/Util/Application.h>
#include <Poco/Util/OptionSet.h>
#include <array>
#include <chrono>
#include <string>
#include <vector>

namespace remwharead_cli
{
using namespace remwharead;
using std::string;
using std::vector;
using std::array;
using std::chrono::system_clock;
using time_point = system_clock::time_point;
using Poco::Util::OptionSet;

class App : public Poco::Util::Application
{
public:
    App();

protected:
    void defineOptions(OptionSet& options) override;
    void handle_options(const string &name, const string &value);
    void print_help(const string &option);
    static void print_version();
    int main(const std::vector<string> &args) override;

private:
    bool _exit_requested;
    bool _argument_error;
    string _uri;
    vector<string> _tags;
    export_format _format;
    string _file;
    array<time_point, 2> _timespan;
    string _search_tags;
    string _search_all;
    bool _archive;
    bool _regex;
};
} // namespace remwharead_cli

#endif  // REMWHAREAD_PARSE_OPTIONS_HPP
