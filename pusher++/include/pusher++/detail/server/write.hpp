//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PUSHERPP_DETAIL_SERVER_WRITE_HPP
#define PUSHERPP_DETAIL_SERVER_WRITE_HPP

#include "crypto.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <sstream>
#include <boost/beast/http.hpp>

namespace pusher { namespace detail { namespace server
{
    inline std::string make_body(std::string const& channel, std::string const& event, std::string const& data)
    {
        rapidjson::StringBuffer body_buf;
        rapidjson::Writer<rapidjson::StringBuffer> body_writer{body_buf};
        body_writer.StartObject();
        body_writer.String("name");
        body_writer.String(event);
        body_writer.String("channel");
        body_writer.String(channel);
        body_writer.String("data");
        body_writer.String(data);
        body_writer.EndObject();
        return {body_buf.GetString(), body_buf.GetSize()};
    }

    inline std::string make_url(std::string const& app_id, std::string const& key, std::string const& secret, std::string const& body)
    {
        std::stringstream url_builder;
        url_builder << "/apps/" << app_id << "/events";
        std::stringstream request_builder;
        request_builder
            << "auth_key=" << key
            << "&auth_timestamp=" << pusher::detail::clock::to_time_t(pusher::detail::clock::now())
            << "&auth_version=1.0"
            << "&body_md5=" << md5_digest(body);
        std::string to_sign = "POST\n" + url_builder.str() + '\n' + request_builder.str();
        request_builder << "&auth_signature=" << auth_signature(to_sign, secret);
        url_builder << '?' << request_builder.str();
        return url_builder.str();
    }

    inline auto make_request(std::string const& host_and_port, std::string url, std::string body)
    {
        namespace http = boost::beast::http;
        http::request<http::string_body> req{http::verb::post, url, 11};
        req.set(http::field::host, host_and_port);
        req.set(http::field::content_type, "application/json");
        req.body = body;
        req.prepare_payload();
        return req;
    }

}}} // pusher::detail::server

#endif // PUSHERPP_DETAIL_SERVER_WRITE_HPP
