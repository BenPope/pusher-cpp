#          Copyright Ben Pope 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.3)

find_package(Boost REQUIRED COMPONENTS system)

add_library(pusher++ INTERFACE)
target_sources(pusher++ INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pusher++/client.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pusher++/channel_proxy.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pusher++/event.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pusher++/server.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pusher++/detail/client/read.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pusher++/detail/client/signal_filter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pusher++/detail/client/write.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pusher++/detail/server/crypto.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pusher++/detail/server/write.hpp
)
target_include_directories(pusher++ INTERFACE
    ${Boost_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/Beast/include
    ${PROJECT_SOURCE_DIR}/rapidjson-1.1.0/include
    ${PROJECT_SOURCE_DIR}/pusher++/include
)
target_compile_definitions(pusher++ INTERFACE
    -DCRYPTOPP_ENABLE_NAMESPACE_WEAK=1
    -DRAPIDJSON_HAS_STDSTRING=1
)
target_compile_features(pusher++ INTERFACE
    cxx_lambda_init_captures # forces C++14
)
target_link_libraries(pusher++ INTERFACE
    ${Boost_LIBRARIES}
    cryptopp
)

add_subdirectory(examples)
add_subdirectory(test)
