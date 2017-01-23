//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PUSHERPP_EVENT_HPP
#define PUSHERPP_EVENT_HPP

#include <chrono>
#include <string>

namespace pusher
{
    namespace detail
    {
        using clock = std::chrono::system_clock;
    }

    struct event
    {
        std::string channel;
        std::string name;
        std::string data;
        detail::clock::time_point timestamp;
    };
}

#endif // PUSHERPP_EVENT_HPP
