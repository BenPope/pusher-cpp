#          Copyright Ben Pope 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

include(DownloadProject)
download_project(
    PROJ                 Beast
    GIT_REPOSITORY       https://github.com/boostorg/beast.git
    GIT_TAG              ${BEAST_GIT_TAG}
    UPDATE_DISCONNECTED  1
    SOURCE_DIR           ${CMAKE_CURRENT_SOURCE_DIR}/Beast
)

add_library(Beast INTERFACE)
target_include_directories(Beast INTERFACE
    ${Beast_SOURCE_DIR}/include
    ${Beast_SOURCE_DIR}/extras
)
