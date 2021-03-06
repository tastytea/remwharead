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

#ifndef REMWHAREAD_EXPORT_EXPORT_HPP
#define REMWHAREAD_EXPORT_EXPORT_HPP

#include "sqlite.hpp"
#include <iostream>
#include <list>

namespace remwharead::Export
{
using std::list;
using std::ostream;
using std::cout;
/*!
 *  @brief  Base class for exports.
 *
 *  @since  0.6.0
 *
 *  @headerfile export.hpp remwharead/export/export.hpp
 */
class ExportBase
{
public:
    /*!
     *  @brief  Export list of Database::entry.
     *
     *  @param  entries List of Database::entry to export.
     *  @param  out     Output stream.
     */
    explicit ExportBase(const list<Database::entry> &entries,
                        ostream &out = cout);
    virtual ~ExportBase() = default;
    ExportBase(const ExportBase &) = delete;
    ExportBase &operator=(const ExportBase &) = delete;
    ExportBase(ExportBase &&) = delete;
    ExportBase &operator=(ExportBase &&) = delete;


    /*!
     *  @brief  Print output to std::ostream.
     */
    virtual void print() const = 0;

protected:
    const list<Database::entry> _entries;
    ostream &_out;

    /*!
     *  @brief  Sort entries from newest to oldest and remove duplicates.
     *
     *  @param  entries List of Database::entry to sort.
     *
     *  @return Sorted list of Database::entry.
     */
    [[nodiscard]]
    static list<Database::entry> sort_entries(list<Database::entry> entries);
};
} // namespace remwharead::Export

#endif  // REMWHAREAD_EXPORT_EXPORT_HPP
