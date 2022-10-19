
set(THIS garlic-daemon)

include_directories(include)

#for arch
include_directories(/usr/include/dbus-1.0)
include_directories(/usr/lib/dbus-1.0/include/)

set(SOURCE_FILES
        source/watchdog.cpp
        source/dbus_server.cpp
        source/daemon.cpp
	source/main.cpp)

set (CMAKE_CXX_STANDARD 17)

# Make build flags compiler specific
if (CMAKE_COMPILER_IS_GNUCC)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set (CMAKE_C_FLAGS "-D_REETRANT -ggdb -Wall -Wextra -pedantic -O0")
	elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
		set (CMAKE_C_FLAGS "-D_REETRANT -DNDEBUG -Wall -Wextra -pedantic -O3")
	endif ()
endif (CMAKE_COMPILER_IS_GNUCC)


add_executable(${THIS} ${SOURCE_FILES})

target_link_libraries(${THIS} libconfig++.so libdbus-1.so)

# Install the config file
install(FILES resource/config/garlic-daemon.conf DESTINATION /etc/garlic-daemon)

# Intall the systemd file
install(FILES resource/systemd/garlic-daemon.service DESTINATION /etc/systemd/system)

# Install the binary program
install(TARGETS ${THIS} DESTINATION /usr/bin)
