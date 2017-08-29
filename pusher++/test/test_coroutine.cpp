//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <boost/asio/spawn.hpp>
#include <boost/system/error_code.hpp>

#include <pusher++/client.hpp>
#include <pusher++/server.hpp>

BOOST_AUTO_TEST_CASE(test_coroutine_tx_and_rx)
{
    std::string const tx_message = "Message";
    std::string rx_message;
    boost::asio::io_service ios;

    pusher::client<boost::asio::ip::tcp::socket> client{ios, key(), cluster()};
    pusher::server<boost::asio::ip::tcp::socket> server{ios, app_id(), key(), secret(), cluster()};

    boost::asio::spawn(ios, [&](boost::asio::yield_context yield)
    {
        boost::system::error_code ec;

        client.async_connect(yield[ec]);
        BOOST_REQUIRE_EQUAL(ec, boost::system::errc::success);

        server.async_connect(yield[ec]);
        BOOST_REQUIRE_EQUAL(ec, boost::system::errc::success);

        auto channel_sub = client.channel("test_channel");
        channel_sub.bind("test_name", [&client, &rx_message](pusher::event const& ev)
        {
            rx_message = ev.data;
            client.disconnect();
        });

        auto response = server.trigger("test_channel", "test_name", tx_message, yield[ec]);
        BOOST_REQUIRE_EQUAL(ec, boost::system::errc::success);
        BOOST_REQUIRE_EQUAL(response.result(), boost::beast::http::status::ok);

        server.disconnect();
    });
    ios.run();

    BOOST_REQUIRE_EQUAL(tx_message, rx_message);
}
