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

#include <map>
#include <regex>
#include <algorithm>
#include <utility>
#include <cstdint>
#include <curlpp/cURLpp.hpp>
#include "version.hpp"
#include "time.hpp"
#include "export.hpp"

using std::cerr;
using std::endl;
using std::uint64_t;
using std::regex;
using std::regex_replace;
using std::pair;

void export_adoc(const vector<Database::entry> &entries, ostream &out)
{
    try
    {
        out << "= Visited things\n"
            << ":Author: remwharead " << global::version << endl
            << ":Date: " << timepoint_to_string(system_clock::now()) << endl
            << ":toc: right" << endl
            // << ":toc-title:" << endl
            << endl;

        std::map<string,uint64_t> alltags;
        string day;
        for (const Database::entry &entry : entries)
        {
            const string datetime = timepoint_to_string(entry.datetime);
            const string newday = datetime.substr(0, datetime.find('T'));
            const string time = datetime.substr(datetime.find('T') + 1);
            if (newday != day)
            {
                day = newday;
                out << "== " << day << endl << endl;
            }
            out << ".link:" << entry.uri;
            if (!entry.title.empty())
            {
                out << '[' << entry.title << ']';
            }
            else
            {
                out << "[]";
            }
            out << endl;
            out << '_' << time << '_';
            for (const string &tag : entry.tags)
            {
                if (tag.empty())
                {
                    continue;
                }

                auto globaltag = alltags.find(tag);
                if (globaltag != alltags.end())
                {
                    ++(globaltag->second);
                }
                else
                {
                    alltags.insert({ tag, 1 });
                }

                out << " xref:" << replace_spaces(tag) << '[' << tag << ']';
                if (tag != *(entry.tags.rbegin()))
                {
                    out << ',';
                }
            }
            out << endl;
            if (!entry.archive_uri.empty())
            {
                out << " (" << entry.archive_uri << "[archived version])\n";
            }
            out << endl;
            if (!entry.description.empty())
            {
                out << entry.description << endl << endl;
            }
        }

        if (!alltags.empty())
        {
            out << "== Tags\n\n";
            vector<pair<string,uint64_t>> sortedtags(alltags.size());
            std::move(alltags.begin(), alltags.end(), sortedtags.begin());
            std::sort(sortedtags.begin(), sortedtags.end(),
                      [](const pair<string,uint64_t> &a, pair<string,uint64_t> &b)
                      {
                          return a.second > b.second;
                      });

            for (const auto &tag : sortedtags)
            {
                out << "[[" << replace_spaces(tag.first) << "]]" << tag.first
                    << "(" << tag.second << ")";
                if (tag != *(sortedtags.rbegin()))
                {
                    out << ", ";
                }
            }
            out << endl;
        }
    }
    catch (std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
    }
}

const string replace_spaces(const string &text)
{
    return regex_replace(text, regex(" "), "-");
}
