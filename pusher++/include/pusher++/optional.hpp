//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PUSHERPP_OPTIONAL_HPP
#define PUSHERPP_OPTIONAL_HPP

#ifdef __has_include
#  if __has_include(<optional>)
#    include <optional>
#    define have_optional 1
#  elif __has_include(<experimental/optional>)
#    include <experimental/optional>
#    define have_optional 1
#    define experimental_optional
#  endif
#endif
#ifndef have_optional
#    include "impl/optional.hpp"
#    define have_optional 1
#    define experimental_optional
#endif

namespace pusher
{
    namespace detail
    {
#ifdef experimental_optional
        using std::experimental::optional;
        using std::experimental::make_optional;
        constexpr auto& nullopt = std::experimental::nullopt;
#else
        using std::optional;
        using std::make_optional;
        constexpr auto& nullopt = std::nullopt;
#endif
    }
}

#endif // PUSHERPP_OPTIONAL_HPP
