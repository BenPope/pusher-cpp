//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PUSHERPP_TEST_TEST_HPP
#define PUSHERPP_TEST_TEST_HPP

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API

#include <boost/test/unit_test.hpp>
#include <iosfwd>
#include <string>

namespace pusher
{
    class event;
}

std::string app_id();
std::string key();
std::string secret();
std::string cluster();

std::ostream& operator<<(std::ostream& os, pusher::event const& ev);

#endif // PUSHERPP_TEST_TEST_HPP
