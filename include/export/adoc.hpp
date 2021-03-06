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

#ifndef REMWHAREAD_EXPORT_ADOC_HPP
#define REMWHAREAD_EXPORT_ADOC_HPP

#include "export.hpp"
#include "sqlite.hpp"
#include <map>
#include <string>
#include <vector>

namespace remwharead::Export
{
using std::string;

/*!
 *  @brief  Export as %AsciiDoc document.
 *
 *  @since  0.6.0
 *
 *  @headerfile adoc.hpp remwharead/export/adoc.hpp
 */
class AsciiDoc : protected ExportBase
{
public:
    using ExportBase::ExportBase;

    void print() const override;

private:
    using tagmap = std::map<string, list<Database::entry>>;
    using replacemap = const std::map<const string, const string>;

    //! Replace strings in text.
    [[nodiscard]]
    static string replace(string text, const replacemap &replacements);

    //! Replaces characters in tags that asciidoctor doesn't like.
    [[nodiscard]]
    static string replace_in_tag(const string &text);

    //! Replaces characters in title that asciidoctor doesn't like.
    [[nodiscard]]
    static string replace_in_title(const string &text);

    //! Replaces characters in URI that asciidoctor doesn't like.
    [[nodiscard]]
    static string replace_in_uri(const string &text);

    //! Print things sorted by tag.
    void print_tags(const tagmap &tags) const;

    //! Get ISO-8601 day from Database::entry.
    [[nodiscard]]
    static string get_day(const Database::entry &entry);

    //! Get ISO-8601 time from Database::entry.
    [[nodiscard]]
    static string get_time(const Database::entry &entry);
};
} // namespace remwharead::Export

#endif  // REMWHAREAD_EXPORT_ADOC_HPP
