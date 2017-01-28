#          Copyright Ben Pope 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

include(DownloadProject)
download_project(
    PROJ                 date
    GIT_REPOSITORY       https://github.com/HowardHinnant/date.git
    GIT_TAG              ${DATE_GIT_TAG}
    UPDATE_DISCONNECTED  1
    SOURCE_DIR           ${CMAKE_CURRENT_SOURCE_DIR}/date
)

add_library(date INTERFACE)
target_include_directories(date INTERFACE
    ${date_SOURCE_DIR}/..
)

add_library(tz
    ${date_SOURCE_DIR}/tz.cpp
)
target_compile_features(tz PUBLIC
    cxx_lambda_init_captures # forces C++14
)
target_link_libraries(tz INTERFACE
    date
)
