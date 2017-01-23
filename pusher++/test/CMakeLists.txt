#          Copyright Ben Pope 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

find_package(Boost REQUIRED COMPONENTS unit_test_framework program_options coroutine system)
find_package(CURL REQUIRED)

set(common_link_libraries
    pusher++
    ${Boost_LIBRARIES}
    tz
    ${CURL_LIBRARIES}
    cryptopp
    pthread
)

add_executable(${PROJECT_NAME}-test
    test.hpp
    test.cpp
    test_coroutine.cpp
    test_signal_filter.cpp
)
target_link_libraries(${PROJECT_NAME}-test ${common_link_libraries})
#target_include_directories(${PROJECT_NAME}-test PUBLIC
#   ${CMAKE_CURRENT_SOURCE_DIR}
#)
