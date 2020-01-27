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

#include <string>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>

using std::string;
using std::cin;
using std::cout;
using std::uint32_t;
using std::system;

string read_input();
void send_message(const string &message);
int launch(const string &args);
string decode_args(const string &args);
void replace_in_field(string &field);

string read_input()
{
    // Read message length.
    uint32_t length;
    char buffer[4];

    cin.read(buffer, sizeof(uint32_t));
    std::memcpy(&length, buffer, 4);

    // Ignore quotes.
    length -= 2;
    cin.ignore(1);

    // Read message.
    string input;
    char c;

    for (; length > 0; --length)
    {
        cin.read(&c, 1);
        input += c;
    }

    return input;
}

void send_message(const string &message)
{
    const auto length = static_cast<uint32_t>(message.length() + 2);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast) - Necessary.
    cout.write(reinterpret_cast<const char*>(&length), sizeof(uint32_t));
    cout << '"' << message << '"';
}

int launch(const string &args)
{
    const string cmd = "remwharead " + args + " 2>/dev/null";
    // NOLINTNEXTLINE(cert-env33-c) - We get the arguments in a string.
    int ret = system(cmd.c_str());
    if (WIFEXITED(ret))         // NOLINT(hicpp-signed-bitwise) - Necessary.
    {
        ret = WEXITSTATUS(ret); // NOLINT(hicpp-signed-bitwise) - Necessary.
    }

    return ret;
}

string decode_args(const string &args)
{
    constexpr char separator{'\u001F'}; // UNIT SEPARATOR.
    if (args[0] != separator)           // Extension uses old method.
    {
        return args;
    }

    size_t pos{1};
    size_t endpos{0};
    string newargs;
    while ((endpos = args.find(separator, pos)) != string::npos)
    {
        string field{args.substr(pos, endpos - pos)};
        replace_in_field(field);
        newargs += " \"" + field + '"';
        pos = endpos + 1;
    }

    return newargs;
}

void replace_in_field(string &field)
{
    size_t pos{0};
    while ((pos = field.find('"', pos)) != string::npos)
    {
        field.replace(pos, 1, R"(\")"); // Replace " with \".
        pos += 2;
    }
}

int main()
{
    const string args = read_input();

    const int ret = launch(decode_args(args));

    if (ret == 0)
    {
        send_message("Command successful.");
    }
    else
    {
        send_message("Command failed with status: "
                     + std::to_string(ret) + '.');
    }

    return ret;
}
