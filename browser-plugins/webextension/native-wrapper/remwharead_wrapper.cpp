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
#include <experimental/filesystem>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>

using std::string;
using std::cin;
using std::cout;
using std::uint32_t;
using std::system;

namespace fs = std::experimental::filesystem;

const string read_input()
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
    uint32_t length = message.length() + 2;
    cout.write(reinterpret_cast<const char*>(&length), sizeof(uint32_t));
    cout << '"' << message << '"';
}

int launch(const string &args)
{
    const string cmd = "remwharead " + args + " 2>/dev/null";
    int ret = system(cmd.c_str());
    if (WIFEXITED(ret))
    {
        ret = WEXITSTATUS(ret);
    }

    return ret;
}

int main()
{
    const string args = read_input();

    int ret = launch(args);

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
