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

#ifndef CURL_WRAPPER_TYPES_HPP
#define CURL_WRAPPER_TYPES_HPP

#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>

namespace curl_wrapper
{

using std::ostream;
using std::string;
using std::string_view;

/*!
 *  @brief  The HTTP method.
 *
 *  @since  0.1.0
 */
enum class http_method
{
    DELETE,
    GET,
    HEAD,
    PATCH,
    POST,
    PUT
};

/*!
 *  @brief  Return type for network requests.
 *
 *  Currently only HTTP is considered.
 *
 *  @since  0.1.0
 */
struct answer
{
    std::uint16_t status{0}; //!< Status code.
    string headers;          //!< The headers of the response from the server.
    string body;             //!< The response from the server.

    /*!
     *  @brief  Returns true if #status is 200.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] inline explicit operator bool() const
    {
        return (status == 200);
    }

    /*!
     *  @brief  Returns std::string_view of the #body.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] inline explicit operator string_view() const
    {
        return body;
    }

    /*!
     *  @brief  Returns #body as std::ostream.
     *
     *  @since  0.1.0
     */
    inline friend ostream &operator<<(ostream &out, const answer &answer)
    {
        out << answer.body;
        return out;
    }

    /*!
     *  @brief  Returns the value of a header field.
     *
     *  @param  field Case insensitive, ASCII only.
     *
     *  @return A std::string_view to the value of the header field or {} if not
     *          found.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] string_view get_header(string_view field) const;
};

} // namespace curl_wrapper

#endif // CURL_WRAPPER_TYPES_HPP
