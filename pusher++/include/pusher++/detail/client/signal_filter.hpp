//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PUSHERPP_DETAIL_CLIENT_SIGNAL_FILTER_HPP
#define PUSHERPP_DETAIL_CLIENT_SIGNAL_FILTER_HPP

#include <boost/signals2.hpp>
#include <map>
#include <string>
#include <type_traits>
#include <utility>

#include <pusher++/event.hpp>

namespace pusher { namespace detail { namespace client
{
    // Thread safe signals are not required
    using signal_mutex = boost::signals2::keywords::mutex_type<boost::signals2::dummy_mutex>;
    using signal = boost::signals2::signal_type<void(event const&), signal_mutex>::type;

    using signal_map = std::map<std::string, signal>;
    using scoped_connection = boost::signals2::scoped_connection;

    template<typename FilterT>
    class signal_filter
    {
    public:
        signal* source_;
        std::decay_t<FilterT> filter_;
        signal_map filtered_;

        explicit signal_filter(FilterT&& filter)
          : source_{nullptr}
          , filter_{std::forward<FilterT>(filter)}
          , filtered_{}
        {}

        auto connect_source(signal& source)
        {
            source_ = &source;
            source_->connect([this](event const& ev)
            {
                auto name = filter_(ev);
                if(!name.empty())
                {
                    auto it = filtered_.find(name);
                    if(it != std::end(filtered_))
                        it->second(ev);
                }
            });
        }

        template<typename FuncT>
        auto connect(FuncT&& func)
        {
            return source_->connect(std::forward<FuncT>(func));
        }

        template<typename FuncT>
        auto connect(std::string const& name, FuncT&& func)
        {
            return filtered_[name].connect(std::forward<FuncT>(func));
        }
    };

    template<typename FilterT>
    signal_filter<FilterT> filtered_signal(FilterT&& filter)
    {
        return signal_filter<FilterT>{std::forward<FilterT>(filter)};
    }

    inline std::string by_channel(event const& ev)
    {
        return ev.channel;
    }

    inline std::string by_name(event const& ev)
    {
        return ev.name;
    }

}}} // pusher::detail::client

#endif // PUSHERPP_DETAIL_CLIENT_SIGNAL_FILTER_HPP
