#          Copyright Ben Pope 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.3)
project(pusher++_super CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

add_library(tz date/tz.cpp)
target_include_directories(tz INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_features(tz PUBLIC
    cxx_lambda_init_captures # forces C++14
)

add_subdirectory(pusher++)
