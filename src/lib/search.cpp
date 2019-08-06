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
#include <Poco/UTF8String.h>
#include "search.hpp"

namespace remwharead
{
    using std::regex;
    using std::regex_search;
    using std::smatch;
    using std::find;
    using std::find_if;

    Search::Search(const vector<Database::entry> &entries)
        :_entries(entries)
    {}

    const vector<vector<string>> Search::parse_expression(string expression)
        const
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

    const string Search::to_lowercase(const string &str) const
    {
        return Poco::UTF8::toLower(str);
    }

    const vector<DB::entry> Search::search_tags(string expression,
                                                const bool is_re) const
    {
        vector<vector<string>> searchlist = parse_expression(expression);
        vector<DB::entry> result;

        for (const vector<string> &tags_or : searchlist)
        {
            for (const DB::entry &entry : _entries)
            {           // Add entry to result if all tags in an OR-slice match.
                bool matched = true;

                for (const string &tag : tags_or)
                {
                    const auto it = find_if(
                        entry.tags.begin(), entry.tags.end(),
                        [&, is_re](string s)
                        {
                            s = to_lowercase(s);
                            if (is_re)
                            {
                                const regex re("^" + tag + "$");
                                return regex_search(s, re);
                            }
                            else
                            {
                                return (s == tag);
                            }
                        });
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

    const vector<DB::entry> Search::search_all(string expression,
                                               const bool is_re) const
    {
        vector<vector<string>> searchlist = parse_expression(expression);
        vector<DB::entry> result = search_tags(expression, is_re);

        for (const vector<string> &terms_or : searchlist)
        {
            for (const DB::entry &entry : _entries)
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
                    const string title = to_lowercase(entry.title);
                    const string description = to_lowercase(entry.description);
                    const string fulltext = to_lowercase(entry.fulltext);

                    // Set matched_* to false if term is not found.
                    if (is_re)
                    {
                        const regex re(term);

                        if(!regex_search(title, re))
                        {
                            matched_title = false;
                        }

                        if(!regex_search(description, re))
                        {
                            matched_description = false;
                        }

                        if(!regex_search(fulltext, re))
                        {
                            matched_fulltext = false;
                        }
                    }
                    else
                    {
                        if (title.find(term) == string::npos)
                        {
                            matched_title = false;
                        }

                        if (description.find(term) == string::npos)
                        {
                            matched_description = false;
                        }

                        if (fulltext.find(term) == string::npos)
                        {
                            matched_fulltext = false;
                        }
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
}
