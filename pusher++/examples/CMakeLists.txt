#          Copyright Ben Pope 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

find_package(Boost REQUIRED COMPONENTS program_options coroutine system)
find_package(CURL REQUIRED)

set(common_link_libraries
    pusher++
    ${Boost_LIBRARIES}
    tz
    ${CURL_LIBRARIES}
    cryptopp
    pthread
)

add_executable(${PROJECT_NAME}-server server.cpp)
target_link_libraries(${PROJECT_NAME}-server ${common_link_libraries})

add_executable(${PROJECT_NAME}-client client.cpp)
target_link_libraries(${PROJECT_NAME}-client ${common_link_libraries})
