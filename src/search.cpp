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
#include <regex>
#include <algorithm>
#include "search.hpp"

using std::cerr;
using std::endl;
using std::regex;
using std::regex_search;
using std::smatch;
using std::find;

const vector<Database::entry>
search_tags(const vector<Database::entry> &entries, string expression)
{
    vector<vector<string>> searchlist;
    const regex re_or("(.+?) (OR|\\|\\|) ");
    const regex re_and("(.+?) (AND|&&) ");
    smatch match;
    vector<Database::entry> result;

    vector<string> subexpressions;
    {                           // Split expression at OR.
        while (regex_search(expression, match, re_or))
        {
            subexpressions.push_back(match[1].str());
            expression = match.suffix().str();
        }
        subexpressions.push_back(expression);
    }

    {
        for (string sub : subexpressions)
        {                       // Split each OR-slice at AND.
            vector<string> tags;
            while (regex_search(sub, match, re_and))
            {
                tags.push_back(match[1].str());
                sub = match.suffix().str();
            }
            tags.push_back(sub);
            searchlist.push_back(tags);
        }
    }

    for (const vector<string> &tags_or : searchlist)
    {
        for (const Database::entry &entry : entries)
        {               // Add entry to result if all tags in an OR-slice match.
            bool matched = true;
            for (const string &tag : tags_or)
            {
                const auto it = find(entry.tags.begin(), entry.tags.end(), tag);
                if (it == entry.tags.end())
                {
                    matched = false;
                }
            }
            if (matched == true)
            {
                result.push_back(entry);
            }
        }
    }

    return result;
}
