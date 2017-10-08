//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PUSHERPP_DETAIL_CLIENT_READ_HPP
#define PUSHERPP_DETAIL_CLIENT_READ_HPP

#include <boost/asio/buffers_iterator.hpp>
#include <boost/beast/core/ostream.hpp>
#include <boost/beast/core/string.hpp>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>

#include <pusher++/event.hpp>

namespace pusher { namespace detail { namespace client
{
    template<typename ValueT>
    std::string stringify(ValueT const& value)
    {
        if (value.IsString())
            return value.GetString();

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        if(!value.Accept(writer))
            return std::string();

        auto begin = buffer.GetString();
        auto size = buffer.GetSize();
        return std::string(begin, size);
    }

    inline event make_event(boost::beast::flat_buffer const& buf)
    {
        rapidjson::Document d;

        d.Parse(&*boost::asio::buffers_begin(buf.data()), buf.size());
        event ev{};
        if(d.HasMember("channel"))
            ev.channel = d["channel"].GetString();
        if(d.HasMember("event"))
            ev.name = d["event"].GetString();
        if(d.HasMember("data"))
            ev.data = stringify(d["data"]);
        ev.timestamp = detail::clock::now();
        return ev;
    }
}}} // pusher::detail::client

#endif // PUSHERPP_DETAIL_CLIENT_READ_HPP
