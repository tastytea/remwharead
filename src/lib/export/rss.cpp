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

#include "export/rss.hpp"
#include "time.hpp"
#include "version.hpp"
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/SAX/AttributesImpl.h>
#include <Poco/Timestamp.h>
#include <Poco/XML/XMLWriter.h>
#include <cstdint>
#include <ctime>

namespace remwharead
{
using std::cerr;
using std::endl;
using std::time_t;
using Poco::XML::XMLWriter;
using Poco::XML::AttributesImpl;
using Poco::DateTime;
using Poco::DateTimeFormatter;
using Poco::Timestamp;

void Export::RSS::print() const
{
    try
    {
        XMLWriter writer(_out, XMLWriter::CANONICAL);
        AttributesImpl attrs_rss;
        AttributesImpl attrs_guid;
        constexpr char timefmt_rfc822[] = "%w, %d %b %Y %H:%M:%S %Z";

        attrs_rss.addAttribute("", "", "version", "", "2.0");
        attrs_rss.addAttribute("", "", "xmlns:atom", "",
                               "http://www.w3.org/2005/Atom");
        attrs_guid.addAttribute("", "", "isPermaLink", "", "false");

        writer.startDocument();
        writer.startElement("", "", "rss", attrs_rss);
        writer.startElement("", "", "channel");

        writer.startElement("", "", "title");
        writer.characters("Visited things");
        writer.endElement("", "", "title");

        writer.startElement("", "", "link");
        writer.endElement("", "", "link");

        writer.startElement("", "", "description");
        writer.characters("Export from remwharead.");
        writer.endElement("", "", "description");

        writer.startElement("", "", "generator");
        writer.characters(string("remwharead ") + global::version);
        writer.endElement("", "", "generator");

        const string now = DateTimeFormatter::format(DateTime(),
                                                     timefmt_rfc822);
        writer.startElement("", "", "lastBuildDate");
        writer.characters(now);
        writer.endElement("", "", "lastBuildDate");

        for (const Database::entry &entry : _entries)
        {
            writer.startElement("", "", "item");

            writer.startElement("", "", "title");
            if (!entry.title.empty())
            {
                writer.characters(entry.title);
            }
            else
            {
                constexpr std::uint8_t maxlen = 100;
                string title = entry.description.substr(0, maxlen);
                if (entry.description.length() > maxlen)
                {
                    title += " […]";
                }
                writer.characters(title);
            }
            writer.endElement("", "", "title");

            writer.startElement("", "", "link");
            writer.characters(entry.uri);
            writer.endElement("", "", "link");

            writer.startElement("", "", "guid", attrs_guid);
            writer.characters(entry.uri + " at "
                              + timepoint_to_string(entry.datetime));
            writer.endElement("", "", "guid");

            const time_t time = system_clock::to_time_t(entry.datetime);
            const string time_visited = DateTimeFormatter::format(
                Timestamp::fromEpochTime(time), timefmt_rfc822);
            writer.startElement("", "", "pubDate");
            writer.characters(time_visited);
            writer.endElement("", "", "pubDate");

            string description = entry.description;
            if (!description.empty())
            {
                description = "<p>" + description.append("</p>");
            }
            if (!entry.tags.empty())
            {
                description += "<p><strong>Tags:</strong> ";
                for (const string &tag : entry.tags)
                {
                    description += tag;
                    if (tag != *(entry.tags.rbegin()))
                    {
                        description += ", ";
                    }
                }
                description += "</p>";
            }
            if (!entry.archive_uri.empty())
            {
                description += "<p><strong>Archived version:</strong> "
                    "<a href=\"" + entry.archive_uri + "\">"
                    + entry.archive_uri + "</a></p>";
            }
            writer.startElement("", "", "description");
            writer.characters(description);
            writer.endElement("", "", "description");

            writer.endElement("", "", "item");
        }

        writer.endElement("", "", "channel");
        writer.endElement("", "", "rss");
        writer.endDocument();
        _out << endl;
    }
    catch (std::exception &e)
    {
        cerr << "Error in " << __func__ << ": " << e.what() << endl;
    }
}
} // namespace remwharead
