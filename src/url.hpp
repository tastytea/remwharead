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

#ifndef REMWHAREAD_URL_HPP
#define REMWHAREAD_URL_HPP

#include <string>

using std::string;

typedef struct html_extract
{
    string title;
    string description;
    string fulltext;
} html_extract;

class URL
{
public:
    explicit URL(const string &url);

    //! Download URL and extract title, description and full text.
    const html_extract get();
    //! Save URL in archive and return URL.
    const string archive();

private:
    string _url;

    const string extract_title(const string &html);
    const string extract_description(const string &html);
    const string strip_html(const string &html);
    const string unescape_html(const string &html);
};

#endif  // REMWHAREAD_URL_HPP
