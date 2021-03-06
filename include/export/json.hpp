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

#ifndef REMWHAREAD_EXPORT_JSON_HPP
#define REMWHAREAD_EXPORT_JSON_HPP

#include "export.hpp"
#include <string>

namespace remwharead::Export
{
using std::string;

/*!
 *  @brief  Export as %JSON array.
 *
 *  @since  0.8.0
 *
 *  @headerfile json.hpp remwharead/export/json.hpp
 */
class JSON : protected ExportBase
{
public:
    using ExportBase::ExportBase;

    void print() const override;
};
} // namespace remwharead::Export

#endif  // REMWHAREAD_EXPORT_JSON_HPP
