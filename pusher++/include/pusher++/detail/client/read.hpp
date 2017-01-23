//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PUSHERPP_DETAIL_CLIENT_READ_HPP
#define PUSHERPP_DETAIL_CLIENT_READ_HPP

#include <beast/core/to_string.hpp>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <string>

#include <pusher++/event.hpp>

namespace pusher { namespace detail { namespace client
{
    template<typename DynamicBuffer>
    std::string consume_string(DynamicBuffer& buf)
    {
        auto data = beast::to_string(buf.data());
        buf.consume(buf.size());
        return data;
    }

    inline event make_event(std::string const& data)
    {
        rapidjson::Document d;
        d.Parse(data.data(), data.size());
        event ev{};
        if(d.HasMember("channel"))
            ev.channel = d["channel"].GetString();
        if(d.HasMember("event"))
            ev.name = d["event"].GetString();
        if(d.HasMember("data"))
            ev.data = d["data"].GetString();
        ev.timestamp = detail::clock::now();
        return ev;
    }
}}} // pusher::detail::client

#endif // PUSHERPP_DETAIL_CLIENT_READ_HPP
