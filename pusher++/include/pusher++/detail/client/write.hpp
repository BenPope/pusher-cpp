//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PUSHERPP_DETAIL_CLIENT_WRITE_HPP
#define PUSHERPP_DETAIL_CLIENT_WRITE_HPP

#include <rapidjson/writer.h>
#include <string>

namespace pusher { namespace detail { namespace client
{
    inline std::string make_subscription(std::string const& channel)
    {
        rapidjson::StringBuffer sub_buf;
        rapidjson::Writer<rapidjson::StringBuffer> sub_writer(sub_buf);
        sub_writer.StartObject();
        sub_writer.String("event");
        sub_writer.String("pusher:subscribe");
        sub_writer.String("data");
        sub_writer.StartObject();
        sub_writer.String("channel");
        sub_writer.String(channel);
        sub_writer.EndObject();
        sub_writer.EndObject();
        return {sub_buf.GetString(), sub_buf.GetSize()};
    }
}}} // pusher::detail::client

#endif // PUSHERPP_DETAIL_CLIENT_WRITE_HPP
