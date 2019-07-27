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

#ifndef REMWHAREAD_CSV_HPP
#define REMWHAREAD_CSV_HPP

#include <string>
#include "export.hpp"

namespace remwharead
{
namespace Export
{
    using std::string;

    class CSV : protected ExportBase
    {
    public:
        using ExportBase::ExportBase;

        virtual void print() const override;

    private:
        //! replaces " with "".
        const string quote(string field) const;
    };
}
}

#endif  // REMWHAREAD_CSV_HPP
