cmake_minimum_required (VERSION 3.2)
project (asciip)

# Set the version
set (asciip_VERSION_MAJOR 0)
set (asciip_VERSION_MINOR 1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wundef -Wfloat-equal")

# Add the headers
include_directories (include)

# Enable testing
enable_testing()

# Set the source directory
file (GLOB ASCIIP_SOURCES "src/*.c")
file (GLOB TEST_SOURCES "test/*.cpp")
set(ASCIIP_TEST_SOURCES ${ASCIIP_SOURCES})
list(REMOVE_ITEM ASCIIP_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

add_executable (asciip ${ASCIIP_SOURCES})
add_executable (asciip_test ${TEST_SOURCES} ${ASCIIP_TEST_SOURCES})

find_package(Cpputest REQUIRED)
include_directories(${CPPUTEST_EXT_INCLUDE_DIR} ${CPPUTEST_INCLUDE_DIR})
set(LIBS ${LIBS} ${CPPUTEST_EXT_LIBRARY} ${CPPUTEST_LIBRARY})
target_link_libraries(asciip_test ${LIBS})

add_test(NAME test_driver
         COMMAND asciip_test -c)