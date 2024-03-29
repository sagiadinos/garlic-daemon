cmake_minimum_required(VERSION 3.14)
project(garlic-daemon LANGUAGES CXX)
set(THIS garlic-daemon)

set(SOURCE_FILES
    source/configuration.hpp
    source/configuration.cpp
    source/cmd_parser.hpp
    source/cmd_parser.cpp
    source/watchdog.hpp
    source/watchdog.cpp
    source/daemon.hpp
    source/daemon.cpp
    source/message_dispatcher.hpp
    source/message_dispatcher.cpp
    source/ipc_message_receiver.hpp
    source/ipc_message_receiver.cpp
    source/log.hpp
    source/main.cpp)

set (CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
endif ()

add_executable(${THIS} ${SOURCE_FILES})

target_link_libraries(${THIS} libconfig++.so)

# Install the config file
install(FILES resource/config/garlic-daemon.conf DESTINATION /etc/garlic-daemon)

# Intall the systemd file
install(FILES resource/systemd/garlic-daemon.service DESTINATION /etc/systemd/system)

# Install the binary program
install(TARGETS ${THIS} DESTINATION /usr/bin)
