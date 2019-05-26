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
#include <sys/wait.h>

using std::string;
using std::cin;
using std::cout;
using std::uint32_t;
using std::system;

namespace fs = std::experimental::filesystem;

const string read_input()
{
    string input;
    char c;

    bool start = false;
    while (cin.read(&c, 1).good())
    {
        if (!start)
        {
            if (c == '"')
            {
                start = true;
            }
            continue;
        }
        if (c != '"')
        {
            input += c;
        }
        else
        {
            break;
        }
    }
    return input;
}

void send_message(const string &message)
{
    uint32_t length = message.length() + 2;
    cout.write(reinterpret_cast<const char*>(&length), sizeof(length));
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
    string args = read_input();
    size_t pos = args.find("TEMPFILE");
    string tmpfile;

    if (pos != string::npos)
    {
        try
        {
            tmpfile = fs::temp_directory_path() / "remwharead.html";
            args.replace(pos, 8, tmpfile);
        }
        catch (const fs::filesystem_error &e)
        {
            send_message("Could not create temporary file.");
            return 3;
        }
    }

    int ret = launch(args);

    if (ret == 0)
    {
        if (!tmpfile.empty())
        {
            send_message("FILE:" + tmpfile);
        }
        else
        {
            send_message("Command successful.");
        }
    }
    else
    {
        send_message("Command failed with status: " + std::to_string(ret) + '.');
    }

    return ret;
}
