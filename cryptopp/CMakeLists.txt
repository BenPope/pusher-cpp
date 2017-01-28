#          Copyright Ben Pope 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

include(DownloadProject)
download_project(
    PROJ                 cryptopp
    GIT_REPOSITORY       https://github.com/weidai11/cryptopp.git
    GIT_TAG              ${CRYPTOPP_GIT_TAG}
    UPDATE_DISCONNECTED  1
    SOURCE_DIR           ${CMAKE_CURRENT_SOURCE_DIR}/crypto++
)

add_library(cryptopp INTERFACE)
target_include_directories(cryptopp INTERFACE
    ${cryptopp_SOURCE_DIR}/..
)
target_compile_definitions(cryptopp INTERFACE
    -DCRYPTOPP_ENABLE_NAMESPACE_WEAK=1 # for MD5
)

set(BUILD_SHARED OFF)
set(BUILD_TESTING OFF)

add_subdirectory(crypto++)

target_link_libraries(cryptopp INTERFACE cryptopp-static)
