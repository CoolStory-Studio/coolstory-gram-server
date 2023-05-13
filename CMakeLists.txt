cmake_minimum_required(VERSION 3.22)
project(coolstory_gram_server)

set(CMAKE_CXX_STANDARD 17)
include(ExternalProject)

set(BOOST_VERSION "1.75.0")
set(BOOST_VERSION_URL "1_75_0")
set(BOOST_URL "https://boostorg.jfrog.io/artifactory/main/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_URL}.tar.bz2")
ExternalProject_Add(
        Boost
        PREFIX ${CMAKE_BINARY_DIR}/boost
        URL ${BOOST_URL}
        SOURCE_DIR ${CMAKE_BINARY_DIR}/boost/src/Boost
        BINARY_DIR ${CMAKE_BINARY_DIR}/boost/src/Boost
        CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/boost/src/Boost/bootstrap.sh
        BUILD_COMMAND ""
        INSTALL_COMMAND ${CMAKE_BINARY_DIR}/boost/src/Boost/b2 install
        --prefix=${CMAKE_BINARY_DIR}/boost/install
        --with-system
        --with-thread
        --with-date_time
        --with-regex
        --with-iostreams
        --with-filesystem
)
add_library(boost_beast INTERFACE)
target_include_directories(boost_beast INTERFACE ${CMAKE_BINARY_DIR}/boost/install/include)
target_link_directories(boost_beast INTERFACE ${CMAKE_BINARY_DIR}/boost/install/lib)
target_link_libraries(boost_beast INTERFACE -lboost_system -lboost_thread -lboost_date_time -lboost_regex -lboost_iostreams -lboost_filesystem)

add_executable(coolstory_gram_server main.cpp)
add_dependencies(coolstory_gram_server Boost)
