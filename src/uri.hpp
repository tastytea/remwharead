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

#ifndef REMWHAREAD_URI_HPP
#define REMWHAREAD_URI_HPP

#include <string>

using std::string;

typedef struct html_extract
{
    string title;
    string description;
    string fulltext;
} html_extract;

class URI
{
public:
    explicit URI(const string &uri);

    //! Download URI and extract title, description and full text.
    const html_extract get();
    //! Save URI in archive and return URI.
    const string archive();

protected:
    string _uri;

    const string extract_title(const string &html);
    const string extract_description(const string &html);
    const string strip_html(const string &html);
    //! Remove all HTML tags. If tag is not empty, remove tag and its content.
    const string remove_html_tags(const string &html, const string &tag = "");
    const string unescape_html(const string &html);
    const string remove_newlines(const string &text);
};

#endif  // REMWHAREAD_URI_HPP
