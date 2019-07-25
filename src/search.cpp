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

#include <regex>
#include <algorithm>
#include <locale>
#include <codecvt>
#include "search.hpp"

using std::regex;
using std::regex_search;
using std::smatch;
using std::find;
using std::find_if;

const vector<vector<string>> parse_expression(string expression)
{
    vector<vector<string>> searchlist;
    const regex re_or("(.+?) (OR|\\|\\|) ");
    const regex re_and("(.+?) (AND|&&) ");
    smatch match;

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
            vector<string> terms;
            while (regex_search(sub, match, re_and))
            {
                terms.push_back(to_lowercase(match[1].str()));
                sub = match.suffix().str();
            }
            terms.push_back(to_lowercase(sub));
            searchlist.push_back(terms);
        }
    }

    return searchlist;
}

const string to_lowercase(const string &str)
{
    string out;
    std::locale loc("");
    const std::ctype<char>& ct = std::use_facet<std::ctype<char>>(loc);
    std::transform(str.begin(), str.end(), std::back_inserter(out),
                   std::bind1st(std::mem_fun(&std::ctype<char>::tolower), &ct));
    return out;
}

const vector<Database::entry>
search_tags(const vector<Database::entry> &entries, string expression)
{
    vector<vector<string>> searchlist = parse_expression(expression);
    vector<Database::entry> result;

    for (const vector<string> &tags_or : searchlist)
    {
        for (const Database::entry &entry : entries)
        {               // Add entry to result if all tags in an OR-slice match.
            bool matched = true;
            for (const string &tag : tags_or)
            {
                const auto it = find_if(entry.tags.begin(), entry.tags.end(),
                                        [&tag](const string &s)
                                        { return to_lowercase(s) == tag; });
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

const vector<Database::entry>
search_all(const vector<Database::entry> &entries, string expression)
{
    vector<vector<string>> searchlist = parse_expression(expression);
    vector<Database::entry> result = search_tags(entries, expression);

    for (const vector<string> &terms_or : searchlist)
    {
        for (const Database::entry &entry : entries)
        {
            // Add entry to result if all terms in an OR-slice match title,
            // description or full text.
            bool matched_title = true;
            bool matched_description = true;
            bool matched_fulltext = true;

            const auto it = find(result.begin(), result.end(), entry);
            if (it != result.end())
            {                   // Skip if already in result list.
                continue;
            }

            for (const string &term : terms_or)
            {
                if (to_lowercase(entry.title).find(term) == string::npos)
                {
                    matched_title = false;
                }

                if (to_lowercase(entry.description).find(term) == string::npos)
                {
                    matched_description = false;
                }

                if (to_lowercase(entry.fulltext).find(term) == string::npos)
                {
                    matched_fulltext = false;
                }
            }
            if (matched_title == true
                || matched_description == true
                || matched_fulltext == true)
            {
                result.push_back(entry);
            }
        }
    }

    return result;
}
