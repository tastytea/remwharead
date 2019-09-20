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

#include <algorithm>
#include <locale>
#include <list>
#include <thread>
#include <utility>
#include <iterator>
#include <Poco/UTF8String.h>
#include <Poco/RegularExpression.h>
#include "search.hpp"

namespace remwharead
{
    using std::list;
    using std::find;
    using std::find_if;
    using std::thread;
    using std::move;
    using RegEx = Poco::RegularExpression;

    Search::Search(const list<Database::entry> &entries)
        :_entries(entries)
    {}

    const vector<vector<string>> Search::parse_expression(string expression)
        const
    {
        vector<vector<string>> searchlist;
        const RegEx re_or("(.+?) (OR|\\|\\|) ");
        const RegEx re_and("(.+?) (AND|&&) ");
        RegEx::MatchVec matches;
        string::size_type pos = 0;

        vector<string> subexpressions;
        {                           // Split expression at OR.
            while (re_or.match(expression, pos, matches) != 0)
            {
                const string &subexpr = expression.substr(matches[1].offset,
                                                          matches[1].length);
                subexpressions.push_back(subexpr);
                pos = matches[0].offset + matches[0].length;
            }
            subexpressions.push_back(expression.substr(pos));
        }

        {
            for (string sub : subexpressions)
            {                       // Split each OR-slice at AND.
                vector<string> terms;
                pos = 0;

                while (re_and.match(sub, pos, matches) != 0)
                {
                    const string &term = sub.substr(matches[1].offset,
                                                    matches[1].length);
                    terms.push_back(to_lowercase(term));
                    pos = matches[0].offset + matches[0].length;
                }
                terms.push_back(to_lowercase(sub.substr(pos)));
                searchlist.push_back(terms);
            }
        }

        return searchlist;
    }

    const string Search::to_lowercase(const string &str) const
    {
        return Poco::UTF8::toLower(str);
    }

    const list<DB::entry> Search::search_tags(string expression,
                                              const bool is_re) const
    {
        vector<vector<string>> searchlist = parse_expression(expression);
        list<DB::entry> result;

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
                                const RegEx re("^" + tag + "$");
                                return (re == s);
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

    const list<DB::entry> Search::search_all(string expression,
                                             const bool is_re) const
    {
        vector<vector<string>> searchlist = parse_expression(expression);
        list<DB::entry> result = search_tags(expression, is_re);

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
                        const RegEx re(term);

                        if (!(re == title))
                        {
                            matched_title = false;
                        }

                        if (!(re == description))
                        {
                            matched_description = false;
                        }

                        if (!(re == fulltext))
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

    const list<Database::entry> Search::search_all_threaded(
        string expression, const bool is_re) const
    {
        list<Database::entry> entries = _entries;

        const size_t len = entries.size();
        constexpr size_t min_len = 100;
        constexpr size_t min_per_thread = 50;
        const size_t n_threads = thread::hardware_concurrency() / 3 + 1;
        size_t cut_at = len;
        if (len > min_len)
        {   // If there are over `min_len` entries, use `n_threads` threads.
            cut_at = len / n_threads;

            // But don't use less than `min_per_thread` entries per thread.
            if (cut_at < min_per_thread)
            {
                cut_at = min_per_thread;
            }
        }

        list<list<Database::entry>> segments;

        // Use threads if list is big.
        while (entries.size() > cut_at)
        {
            list<Database::entry> segment;

            auto it = entries.begin();
            std::advance(it, cut_at);

            // Move the first `cut_at` entries into `segments`.
            segment.splice(segment.begin(), entries, entries.begin(), it);
            segments.push_back(move(segment));
        }
        // Move rest of `entries` into `segments`.
        segments.push_back(move(entries));

        list<thread> threads;
        for (auto &segment : segments)
        {
            thread t(
                [&]
                {
                    Search search(segment);
                    // Replace `segment` with `result`.
                    segment = search.search_all(expression, is_re);
                });
            threads.push_back(move(t));
        }

        for (thread &t : threads)
        {
            t.join();
            // Move each of `segments` into `entries`.
            entries.splice(entries.end(), segments.front());
            segments.pop_front();
        }

        return entries;
    }
}
