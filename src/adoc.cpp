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
#include <locale>
#include <curlpp/cURLpp.hpp>
#include "version.hpp"
#include "time.hpp"
#include "export.hpp"

using std::cerr;
using std::endl;
using std::regex;
using std::regex_replace;
using tagpair = std::pair<string,vector<Database::entry>>;

void export_adoc(const vector<Database::entry> &entries, ostream &out)
{
    try
    {
        out << "= Visited things\n"
            << ":Author: remwharead " << global::version << endl
            << ":Date:   " << timepoint_to_string(system_clock::now()) << endl
            << ":TOC:    right" << endl
            // << ":toc-title:" << endl
            << endl;

        std::map<string,vector<Database::entry>> alltags;
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

            out << "[[dt_" << datetime << "]]\n";
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

            out << '_' << time.substr(0, 5) << '_';
            if (!entry.archive_uri.empty())
            {
                out << " (link:" << entry.archive_uri << "[archived version])";
            }

            bool separator = false;
            for (const string &tag : entry.tags)
            {
                if (tag.empty())
                {
                    continue;
                }
                if (!separator)
                {
                    out << "\n| ";
                    separator = true;
                }

                auto globaltag = alltags.find(tag);
                if (globaltag != alltags.end())
                {
                    globaltag->second.push_back(entry);
                }
                else
                {
                    alltags.insert({ tag, { entry } });
                }

                out << "xref:t_" << replace_in_tags(tag) << "[" << tag << ']';
                if (tag != *(entry.tags.rbegin()))
                {
                    out << ", ";
                }
            }
            out << endl << endl;

            if (!entry.description.empty())
            {
                out <<  entry.description << endl << endl;
            }
        }

        if (!alltags.empty())
        {
            out << "== Tags\n\n";
            vector<tagpair> sortedtags(alltags.size());
            std::move(alltags.begin(), alltags.end(), sortedtags.begin());
            std::sort(sortedtags.begin(), sortedtags.end(),
                      [](const tagpair &a, tagpair &b)
                      {
                          if (a.second.size() != b.second.size())
                          {   // Sort by number of occurrences if they are
                              // different.
                              return a.second.size() > b.second.size();
                          }
                          else
                          {   // Sort by tag names otherwise.
                              std::locale loc("");
                              const std::collate<char> &coll =
                                  std::use_facet<std::collate<char>>(loc);
                              if (coll.compare(a.first.data(), a.first.data()
                                               + a.first.length(),
                                               b.first.data(), b.first.data()
                                               + b.first.length()) == -1)
                              { // -1 == less than
                                  return true;
                              }
                              else
                              {
                                  return false;
                              }
                          }
                      });

            for (const auto &tag : sortedtags)
            {
                out << "=== [[t_" << replace_in_tags(tag.first) << "]]"
                    << tag.first << endl;
                for (const Database::entry &entry : tag.second)
                {
                    string title = entry.title;
                    if (title.empty())
                    {
                        title = "++" + entry.uri + "++";
                    }
                    out << endl << "* xref:dt_"
                        << timepoint_to_string(entry.datetime)
                        << '[' << title << ']' << endl;
                }
                out << endl;
            }
        }
    }
    catch (std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
    }
}

const string replace_in_tags(string text)
{
    // TODO: Find a better solution.
    const std::map<const string, const string> searchreplace =
        {
            { " ", "-" }, { "§", "-" },
            { "$", "-" }, { "%", "-" },
            { "&", "-" }, { "/", "-" },
            { "=", "-" }, { "^", "-" },
            { "!", "-" }, { "?", "-" },
            { "₀", "0" }, { "⁰", "0" },
            { "₁", "1" }, { "¹", "1" },
            { "₂", "2" }, { "²", "2" },
            { "₃", "3" }, { "³", "3" },
            { "₄", "4" }, { "⁴", "4" },
            { "₅", "5" }, { "⁵", "5" },
            { "₆", "6" }, { "⁶", "6" },
            { "₇", "7" }, { "⁷", "7" },
            { "₈", "8" }, { "⁸", "8" },
            { "₉", "9" }, { "⁹", "9" }
        };
    for (const std::pair<const string, const string> &sr : searchreplace)
    {
        size_t pos = 0;
        while ((pos = text.find(sr.first, pos)) != std::string::npos)
        {
            text.replace(pos, sr.first.length(), sr.second);
        }
    }
    return text;
}
