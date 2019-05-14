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

#include <iostream>
#include <string>
#include "sqlite.hpp"
#include "parse_options.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(const int argc, const char *argv[])
{
    options opts = parse_options(argc, argv);
    if (opts.status_code != 0)
    {
        return opts.status_code;
    }

    Database db;
    db ? cout << "success." : cout << "failure.";
    cout << endl;

    db.store("a", "b", "c", "d", "e", "f", "g");

    return 0;
}
