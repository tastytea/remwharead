/*  This file is part of remwharead.
 *  Copyright © 2019, 2020 tastytea <tastytea@tastytea.de>
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

#include <Poco/Message.h>
#include <sys/wait.h>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::string;
using std::system;
using std::uint32_t;

class Message
{
public:
    //! Read message from stdin and store it in _msg.
    Message();

    //! Decode message and return argument string for launch().
    [[nodiscard]] string decode();

private:
    string _msg;

    //! Replace "\u001f" with the char in _msg.
    void unescape();

    //! Replace " with \" in field.
    static void replace_in_field(string &field);
};

//! Send a message back.
void send_message(const string &message);

//! Launch remwharead with args.
int launch(const string &args);

int main()
{
    Message message;
    const auto args{message.decode()};

    const int ret = launch(args);

    if (ret == 0)
    {
        send_message("Command successful.");
    }
    else
    {
        send_message("Command failed with status: " + std::to_string(ret)
                     + '.');
    }

    return ret;
}

Message::Message()
{
    // Read message length.
    uint32_t length{0};
    char buffer[4];
    cin.read(buffer, sizeof(length));
    std::memcpy(&length, buffer, sizeof(length));

    // Ignore quotes.
    length -= 2;
    cin.ignore(1);

    // Read message.
    char c{'\0'};
    for (; length > 0; --length)
    {
        cin.read(&c, 1);
        _msg += c;
    }
}

string Message::decode()
{
    unescape();

    constexpr char separator{'\u001f'}; // UNIT SEPARATOR.
    if (_msg[0] != separator)           // Extension uses old method.
    {
        return _msg;
    }

    size_t pos{1};
    size_t endpos{0};
    string newargs;
    while ((endpos = _msg.find(separator, pos)) != string::npos)
    {
        string field{_msg.substr(pos, endpos - pos)};
        replace_in_field(field);
        newargs += " \"" + field + '"';
        pos = endpos + 1;
    }

    return newargs;
}

void Message::unescape()
{
    size_t pos{0};
    while ((pos = _msg.find(R"(\u001f)", pos)) != string::npos)
    {
        _msg.replace(pos, 6, "\u001f");
    }
}

void Message::replace_in_field(string &field)
{
    size_t pos{0};
    while ((pos = field.find('"', pos)) != string::npos)
    {
        field.replace(pos, 1, R"(\")");
        pos += 2;
    }
}

void send_message(const string &message)
{
    const auto length = static_cast<uint32_t>(message.length() + 2);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    cout.write(reinterpret_cast<const char *>(&length), sizeof(uint32_t));
    cout << '"' << message << '"';
}

int launch(const string &args)
{
    const string cmd = "remwharead " + args + " 2>/dev/null";
    // NOLINTNEXTLINE(cert-env33-c)
    int ret = system(cmd.c_str());
    if (WIFEXITED(ret)) // NOLINT(hicpp-signed-bitwise)
    {
        ret = WEXITSTATUS(ret); // NOLINT(hicpp-signed-bitwise)
    }

    return ret;
}
