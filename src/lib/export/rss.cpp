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

#include <ctime>
#include <Poco/XML/XMLWriter.h>
#include <Poco/SAX/AttributesImpl.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Timestamp.h>
#include "time.hpp"
#include "version.hpp"
#include "export/rss.hpp"

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
            AttributesImpl attrs_rss, attrs_guid;
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
            // FIXME: There has to be an URL here.
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
                writer.characters(entry.title);
                writer.endElement("", "", "title");

                writer.startElement("", "", "link");
                writer.characters(entry.uri);
                writer.endElement("", "", "link");

                writer.startElement("", "", "guid", attrs_guid);
                writer.characters(entry.uri + " at " +
                                  timepoint_to_string(entry.datetime));
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
                    description += "\n\n";
                }
                if (!entry.tags.empty())
                {
                    description += "Tags: ";
                    for (const string &tag : entry.tags)
                    {
                        description += tag;
                        if (tag != *(entry.tags.rbegin()))
                        {
                            description += ", ";
                        }
                    }
                }
                if (!entry.archive_uri.empty())
                {
                    description += "\n\nArchived version: " + entry.archive_uri;
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
}
