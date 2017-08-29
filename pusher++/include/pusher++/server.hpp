//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PUSHERPP_SERVER_HPP
#define PUSHERPP_SERVER_HPP

#include "event.hpp"
#include "detail/server/write.hpp"

#include <boost/asio/async_result.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/handler_type.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http.hpp>
#include <boost/core/swap.hpp>
#include <string>

namespace pusher
{
    template<typename SocketT>
    class server
    {
        using request = boost::beast::http::request<boost::beast::http::string_body>;
        using response = boost::beast::http::response<boost::beast::http::dynamic_body>;

        template<typename Handler, typename Signature>
        using handler_type_t = typename boost::asio::handler_type<Handler, Signature>::type;

        SocketT socket_;
        boost::asio::ip::tcp::resolver resolver_;
        std::string app_id_;
        std::string key_;
        std::string secret_;
        std::string host_;
        request request_;
        response response_;
        boost::beast::flat_buffer response_buf_;

    public:
        server(boost::asio::io_service& ios, std::string app_id, std::string key, std::string secret, std::string cluster = "mt1")
          : socket_{ios}
          , resolver_{ios}
          , app_id_{std::move(app_id)}
          , key_{std::move(key)}
          , secret_{std::move(secret)}
          , host_{"api-" + std::move(cluster) + ".pusher.com"}
        {}

        template<typename TokenT>
        auto async_connect(TokenT&& token)
        {
            handler_type_t<TokenT, void(boost::system::error_code)> handler(std::forward<TokenT>(token));
            boost::asio::async_result<decltype(handler)> result(handler);
            using query = boost::asio::ip::tcp::resolver::query;
            resolver_.async_resolve(query{host_, "http"}, [this, handler](auto ec, auto endpoint) mutable
            {
                if(ec)
                    return handler(ec);

                boost::asio::async_connect(socket_, endpoint, [handler](auto ec, auto) mutable
                {
                    return handler(ec);
                });
            });
            return result.get();
        }

        auto connect()
        {
            return boost::asio::connect(socket_, resolver_.resolve(boost::asio::ip::tcp::resolver::query{host_, "http"}));
        }

        auto disconnect()
        {
            resolver_.cancel();
            socket_.close();
        }

        template<typename TokenT>
        auto trigger(std::string const& channel, std::string const& event, std::string const& data, TokenT&& token)
        {
            handler_type_t<TokenT, void(boost::system::error_code, response)> handler(std::forward<TokenT>(token));
            boost::asio::async_result<decltype(handler)> result(handler);
            auto body = detail::server::make_body(channel, event, data);
            auto url = detail::server::make_url(app_id_, key_, secret_, body);
            auto host_and_port = host_ + ":" + std::to_string(socket_.remote_endpoint().port());
            request_ = detail::server::make_request(host_and_port, url, body);
            
            boost::beast::http::async_write(socket_, request_, [this, handler](auto ec) mutable
            {
                if(ec)
                    return handler(ec, response{});

                boost::beast::http::async_read(socket_, response_buf_, response_, [this, handler](auto ec) mutable
                {
                    response res;
                    boost::swap(res, response_);
                    return handler(ec, res);
                });
            });
            return result.get();
        }

        auto trigger(std::string const& channel, std::string const& event, std::string const& data)
        {
            auto body = detail::server::make_body(channel, event, data);
            auto url = detail::server::make_url(app_id_, key_, secret_, body);
            auto host_and_port = host_ + ":" + std::to_string(socket_.remote_endpoint().port());
            request_ = detail::server::make_request(host_and_port, url, body);

            boost::beast::http::write(socket_, request_);
            boost::beast::http::read(socket_, response_buf_, response_);
            response res;
            boost::swap(res, response_);
            return res;
        }
    };
}

#endif // PUSHERPP_SERVER_HPP
