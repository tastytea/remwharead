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

#ifndef REMWHAREAD_EXPORT_SIMPLE_HPP
#define REMWHAREAD_EXPORT_SIMPLE_HPP

#include "export.hpp"

namespace remwharead
{
namespace Export
{
/*!
 *  @brief  Export as simple list.
 *
 *  @since  0.6.0
 *
 *  @headerfile simple.hpp remwharead/export/simple.hpp
 */
class Simple : protected ExportBase
{
public:
    using ExportBase::ExportBase;
    void print() const override;
};
} // namespace Export
} // namespace remwharead

#endif  // REMWHAREAD_EXPORT_SIMPLE_HPP
