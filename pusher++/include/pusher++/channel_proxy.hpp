//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PUSHERPP_CHANNEL_PROXY_HPP
#define PUSHERPP_CHANNEL_PROXY_HPP

#include "detail/client/signal_filter.hpp"

namespace pusher
{
    class channel_proxy
    {
        using signal_filter = detail::client::signal_filter<std::string(*)(event const&)>;
        signal_filter* signal_filter_;
    public:
        explicit channel_proxy(signal_filter* signal_filter) : signal_filter_{signal_filter}
        {}

        template<typename FuncT>
        auto bind(std::string const& event_name, FuncT&& func)
        {
            return signal_filter_->connect(event_name, std::forward<FuncT>(func));
        }

        template<typename FuncT>
        auto bind_all(FuncT&& func)
        {
            return signal_filter_->connect(std::forward<FuncT>(func));
        }
    };
}

#endif // PUSHERPP_CHANNEL_PROXY_HPP
