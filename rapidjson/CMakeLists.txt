#          Copyright Ben Pope 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

include(DownloadProject)
download_project(
    PROJ                 rapidjson
    GIT_REPOSITORY       https://github.com/miloyip/rapidjson.git
    GIT_TAG              ${RAPIDJSON_GIT_TAG}
    UPDATE_DISCONNECTED  1
    SOURCE_DIR           ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson
)

add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE
    ${rapidjson_SOURCE_DIR}/include
)
target_compile_definitions(rapidjson INTERFACE
    -DRAPIDJSON_HAS_STDSTRING=1
)
