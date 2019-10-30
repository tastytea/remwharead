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

#include <cstdint>
#include <string>

namespace remwharead
{
using std::uint16_t;
using std::string;

/*!
 *  @brief  A processed HTML page.
 *
 *  @return true if successful, when cast to bool.
 *
 *  @since  0.7.0
 *
 *  @headerfile uri.hpp remwharead/uri.hpp
 */
struct html_extract
{
    bool successful = false;
    string error;
    string title;
    string description;
    string fulltext;

    explicit operator bool();
};

/*!
 *  @brief  The result of the call to the archive service.
 *
 *  @return true if successful, when cast to bool.
 *
 *  @since  0.7.0
 *
 *  @headerfile uri.hpp remwharead/uri.hpp
 */
struct archive_answer
{
    bool successful = false;
    string error;
    string uri;

    explicit operator bool();
};

/*!
 *  @brief  Download, archive and process an %URI.
 *
 *  @since  0.6.0
 *
 *  @headerfile uri.hpp remwharead/uri.hpp
 */
class URI
{
public:
    /*!
     *  @brief  Construct object and set URL.
     *
     *  Initializes TLS and sets proxy from the environment variable
     *  `http_proxy`, if possible.
     *
     *  @since  0.6.0
     */
    explicit URI(string uri);
    virtual ~URI();

    URI(const URI &other) = default;
    URI &operator=(const URI &other) = default;
    URI(URI &&other) = default;
    URI &operator=(URI &&other) = default;

    /*!
     *  @brief  Download %URI and extract title, description and full text.
     *
     *  @since  0.6.0
     */
    [[nodiscard]]
    html_extract get();

    /*!
     *  @brief  Save %URI in archive and return archive-URI.
     *
     *  @since  0.6.0
    */
    [[nodiscard]]
    archive_answer archive() const;

protected:
    string _uri;

    /*!
     *  @brief  Make a HTTP(S) request.
     *
     *  @since  0.6.0
     */
    [[nodiscard]]
    string make_request(const string &uri, bool archive = false) const;

    /*!
     *  @brief  Extract the title from an HTML page.
     *
     *  @since  0.6.0
     */
    [[nodiscard]]
    string extract_title(const string &html) const;

    /*!
     *  @brief  Extract the description from an HTML page.
     *
     *  @since  0.6.0
     */
    [[nodiscard]]
    string extract_description(const string &html) const;

    /*!
     *  @brief  Removes HTML tags and superflous spaces from an HTML page.
     *
     *  @since  0.6.0
     */
    [[nodiscard]]
    string strip_html(const string &html) const;

    /*!
     *  @brief  Remove HTML tags.
     *
     *  @param  html HTML page.
     *  @param  tag  If set, only remove this tag.
     *
     *  @since  0.6.0
     */
    [[nodiscard]]
    string remove_html_tags(const string &html, const string &tag = "") const;

    /*!
     *  @brief  Convert HTML entities to UTF-8.
     *
     *  @since  0.6.0
     */
    [[nodiscard]]
    string unescape_html(string html) const;

    /*!
     *  @brief  Replace newlines with spaces.
     *
     *  @since  0.6.0
     */
    [[nodiscard]]
    string remove_newlines(string text) const;

    /*!
     *  @brief  Set proxy server.
     *
     *  @since  0.8.5
     */
    void set_proxy();

    /*!
     *  @brief  Limits text to N characters, cuts at space.
     *
     *  @since  0.8.5
     */
    [[nodiscard]]
    string cut_text(const string &text, uint16_t n_chars) const;
};
} // namespace remwharead

#endif  // REMWHAREAD_URI_HPP
