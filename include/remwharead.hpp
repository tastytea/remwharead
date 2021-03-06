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

#ifndef REMWHAREAD_HPP
#define REMWHAREAD_HPP

/*!
 *  @mainpage remwharead Reference
 *
 *  @section using Using the library
 *
 *  The easiest way is to include remwharead.hpp, which then includes all other
 *  headers.
 *
 *  @code
 *  #include <remwharead/remwharead.hpp>
 *  @endcode
 *
 *  Use it in your CMake project like this:
 *
 *  @code
 *  find_package(remwharead CONFIG REQUIRED)
 *  target_link_libraries(MyProject remwharead::remwharead)
 *  @endcode
 *
 *  Or compile your code with `g++ $(pkg-config --cflags --libs remwharead)`.
 */

#include "export/adoc.hpp"
#include "export/bookmarks.hpp"
#include "export/csv.hpp"
#include "export/export.hpp"
#include "export/json.hpp"
#include "export/rss.hpp"
#include "export/simple.hpp"
#include "export/list.hpp"
#include "export/rofi.hpp"
#include "search.hpp"
#include "sqlite.hpp"
#include "time.hpp"
#include "types.hpp"
#include "uri.hpp"

#endif  // REMWHAREAD_HPP
