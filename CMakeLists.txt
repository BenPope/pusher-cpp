#          Copyright Ben Pope 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.3)
project(pusher++ CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BEAST_GIT_TAG 797631c) # 1.0.0-b24
set(CRYPTOPP_GIT_TAG CRYPTOPP_5_6_5)
set(DATE_GIT_TAG 2.1.0)
set(RAPIDJSON_GIT_TAG v1.1.0)

add_subdirectory(cryptopp)
add_subdirectory(date)
add_subdirectory(rapidjson)
add_subdirectory(Beast)
add_subdirectory(pusher++)
