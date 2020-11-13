/*  This file is part of curl_wrapper.
 *  Copyright © 2020 tastytea <tastytea@tastytea.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "types.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>

namespace curl_wrapper
{

using std::tolower;

std::string_view answer::get_header(const std::string_view field) const
{
    const string searchstring{string(field) += ":"};
    // clang-format off
    auto it{std::search(headers.begin(), headers.end(), searchstring.begin(),
                        searchstring.end(),
                        [](unsigned char a, unsigned char b)
                        { return tolower(a) == tolower(b); })};
    // clang-format on

    if (it != headers.end())
    {
        auto pos{static_cast<size_t>(it - headers.begin())};
        pos = headers.find(':', pos) + 1;
        pos = headers.find_first_not_of(' ', pos);
        const auto endpos{headers.find_first_of("\r\n", pos)};
        return string_view(&headers[pos], endpos - pos);
    }

    return {};
}

} // namespace curl_wrapper
