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

#include "export/adoc.hpp"
#include "time.hpp"
#include "version.hpp"
#include <Poco/URI.h>
#include <algorithm>
#include <iostream>
#include <locale>
#include <string>
#include <utility>

namespace remwharead
{
using std::string;
using std::cerr;
using std::endl;
using tagpair = std::pair<string,list<Database::entry>>;

void Export::AsciiDoc::print() const
{
    try
    {
        _out << "= Visited things\n"
             << ":Author:    remwharead " << version << "\n"
             << ":Date:      "
             << timepoint_to_string(system_clock::now()) << "\n"
             << ":TOC:       right\n"
             << ":TOCLevels: 2\n"
             << ":!webfonts:\n\n";

        tagmap alltags;
        string day;
        for (const Database::entry &entry : _entries)
        {
            const string newday = get_day(entry);

            if (newday != day)
            {
                day = newday;
                _out << "== " << day << endl << endl;
            }

            _out << "[[dt_" << timepoint_to_string(entry.datetime)
                 << "]]\n" << "* link:" << replace_in_uri(entry.uri);
            if (!entry.title.empty())
            {
                _out << '[' << replace_in_title(entry.title) << ']';
            }
            else
            {
                _out << "[]";
            }
            _out << " +" << endl;

            _out << '_' << get_time(entry).substr(0, 5) << '_';
            if (!entry.archive_uri.empty())
            {
                _out << " (link:" << replace_in_uri(entry.archive_uri)
                     << "[archived version])";
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
                    _out << "\n| ";
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

                _out << "xref:t_" << replace_in_tag(tag)
                     << "[" << tag << ']';
                if (tag != *(entry.tags.rbegin()))
                {
                    _out << ", ";
                }
            }

            if (!entry.description.empty())
            {
                _out << " +\n+" << entry.description << '+';
            }
            _out << endl << endl;
        }

        if (!alltags.empty())
        {
            print_tags(alltags);
        }
    }
    catch (std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
    }
}

string Export::AsciiDoc::replace(string text, const replacemap &replacements)
    const
{
    for (const std::pair<const string, const string> &sr : replacements)
    {
        size_t pos = 0;
        while ((pos = text.find(sr.first, pos)) != std::string::npos)
        {
            text.replace(pos, sr.first.length(), sr.second);
            pos += sr.second.length();
        }
    }
    return text;
}
string Export::AsciiDoc::replace_in_tag(const string &text) const
{
    // TODO(tastytea): Find a better solution.
    const replacemap replacements =
        {
            { " ", "-" }, { "§", "-" },
            { "$", "-" }, { "%", "-" },
            { "&", "-" }, { "/", "-" },
            { "=", "-" }, { "^", "-" },
            { "!", "-" }, { "?", "-" },
            { "'", "-" }, { "\"", "-" },
            { "´", "-" }, { "`", "-" },
            { "’", "-" }, { "#", "-" },
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

    return replace(text, replacements);
}

string Export::AsciiDoc::replace_in_title(const string &text) const
{
    // [ is implicitly escaped if the corresponding ] is.
    return replace(text, {{ "]", "\\]" }});
}

string Export::AsciiDoc::replace_in_uri(const string &text) const
{
    string out;
    Poco::URI::encode(text, "+", out);
    return out;
}

void Export::AsciiDoc::print_tags(const tagmap &tags) const
{
    _out << "== Tags\n\n";
    vector<tagpair> sortedtags(tags.size());
    std::move(tags.begin(), tags.end(), sortedtags.begin());

    const auto compare_tags =
        [](const tagpair &a, tagpair &b)
        {
            if (a.second.size() != b.second.size())
            {  // Sort by number of occurrences if they are different.
                return a.second.size() > b.second.size();
            }

            // Sort by tag names otherwise.
            const std::locale loc;
            const auto &coll = std::use_facet<std::collate<char>>(loc);
            return (coll.compare(
                        &a.first[0], &a.first[0] + a.first.size(),
                        &b.first[0], &b.first[0] + b.first.size()) == -1);
        };
    std::sort(sortedtags.begin(), sortedtags.end(), compare_tags);

    bool othertags = false;     // Have we printed “Less used tags” already?
    for (const auto &tag : sortedtags)
    {
        // If we have more than 20 tags, group all tags that occur only 1
        // time under the section “Less used tags”.
        if (sortedtags.size() > 20 && tag.second.size() == 1)
        {
            if (!othertags)
            {
                _out << "=== Less used tags\n\n";
                othertags = true;
            }
            _out << "=";
        }

        _out << "=== [[t_" << replace_in_tag(tag.first) << "]]"
             << tag.first << '\n';
        for (const Database::entry &entry : tag.second)
        {
            const string datetime = timepoint_to_string(entry.datetime);
            const string date = datetime.substr(0, datetime.find('T'));
            string title = replace_in_title(entry.title);
            if (title.empty())
            {
                title = "++" + entry.uri + "++";
            }
            _out << "\n* xref:dt_" << datetime << '[' << title << "] _("
                 << date << ")_" << endl;
        }
        _out << endl;
    }
    _out << endl;
}

string Export::AsciiDoc::get_day(const Database::entry &entry) const
{
    const string datetime = timepoint_to_string(entry.datetime);
    return datetime.substr(0, datetime.find('T'));
}

string Export::AsciiDoc::get_time(const Database::entry &entry) const
{
    const string datetime = timepoint_to_string(entry.datetime);
    return datetime.substr(datetime.find('T') + 1);
}
} // namespace remwharead
