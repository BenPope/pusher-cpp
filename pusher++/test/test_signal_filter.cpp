//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <boost/asio/spawn.hpp>
#include <boost/asio/basic_waitable_timer.hpp>
#include <boost/system/error_code.hpp>
#include <thread>
#include <chrono>

#include <pusher++/client.hpp>
#include <pusher++/server.hpp>

using namespace pusher::detail;

BOOST_AUTO_TEST_CASE(test_signal_filter_all)
{
    auto const time = pusher::detail::clock::now();

    client::signal source;
    auto filtered_signal = client::filtered_signal(client::by_channel);
    filtered_signal.connect_source(source);

    int count = 0;
    auto conn_all = filtered_signal.connect([&count](pusher::event const& ev)
    {
        ++count;
    });
    BOOST_REQUIRE_EQUAL(count, 0);
    source(pusher::event{"one", "one", "one", time});
    BOOST_REQUIRE_EQUAL(count, 1);
    source(pusher::event{"two", "two", "two", time});
    BOOST_REQUIRE_EQUAL(count, 2);
    source(pusher::event{"", "", "", time});
    BOOST_REQUIRE_EQUAL(count, 3);
}

BOOST_AUTO_TEST_CASE(test_signal_filter_filtered)
{
    auto const time = pusher::detail::clock::now();

    client::signal source;
    auto filtered_signal = client::filtered_signal(client::by_channel);
    filtered_signal.connect_source(source);

    int count_one = 0;
    auto conn_one = filtered_signal.connect("one", [&count_one](pusher::event const& ev)
    {
        ++count_one;
    });
    int count_two = 0;
    auto conn_two = filtered_signal.connect("two", [&count_two](pusher::event const& ev)
    {
        ++count_two;
    });

    BOOST_REQUIRE_EQUAL(count_one, 0);
    BOOST_REQUIRE_EQUAL(count_two, 0);

    source(pusher::event{"one", "one", "one", time});
    BOOST_REQUIRE_EQUAL(count_one, 1);
    BOOST_REQUIRE_EQUAL(count_two, 0);

    source(pusher::event{"two", "two", "two", time});
    BOOST_REQUIRE_EQUAL(count_one, 1);
    BOOST_REQUIRE_EQUAL(count_two, 1);

    source(pusher::event{"", "", "", time});
    BOOST_REQUIRE_EQUAL(count_one, 1);
    BOOST_REQUIRE_EQUAL(count_two, 1);
}

BOOST_AUTO_TEST_CASE(test_signal_filter_scoped)
{
    auto const time = pusher::detail::clock::now();

    client::signal source;
    auto filtered_signal = client::filtered_signal(client::by_channel);
    filtered_signal.connect_source(source);

    int count_all = 0;

    {
        auto conn_all = client::scoped_connection(filtered_signal.connect([&count_all](pusher::event const&)
        {
            ++count_all;
        }));

        BOOST_REQUIRE_EQUAL(count_all, 0);

        source(pusher::event{"one", "event", "data", time});
        BOOST_REQUIRE_EQUAL(count_all, 1);
    }

    source(pusher::event{"two", "event", "data", time});
    BOOST_REQUIRE_EQUAL(count_all, 1);
}
