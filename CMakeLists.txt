cmake_minimum_required(VERSION 3.10)
project (
    mntp
    LANGUAGES CXX
    VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)
find_package(Boost REQUIRED COMPONENTS system)
find_package(CURL REQUIRED)

add_executable(
    MNTP-Server
    Server/Main.cpp
    date/src/tz.cpp
)

target_link_libraries(MNTP-Server PRIVATE Boost::system)
target_link_libraries(MNTP-Server PRIVATE CURL::libcurl)
target_include_directories(MNTP-Server PRIVATE date/include)

if (APPLE)
    add_custom_target(
        LIBTZ
        COMMAND make LDLIBS=-lintl
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tzdb-2025b
    )
    add_dependencies(MNTP-Server LIBTZ)
    target_link_directories(MNTP-Server PRIVATE ${CMAKE_SOURCE_DIR}/tzdb-2025b)
    target_link_libraries(MNTP-Server PRIVATE intl tz)
endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

add_executable(
    MNTP-Client
    Client/ClientNew/GetTime.cpp
    Client/ClientNew/Main.cpp
)

target_link_libraries(MNTP-Client PRIVATE Qt5::Widgets Qt5::Core)
target_link_libraries(MNTP-Client PRIVATE Boost::system)
