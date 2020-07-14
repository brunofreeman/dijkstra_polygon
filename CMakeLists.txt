cmake_minimum_required(VERSION 3.16)
project(dijkstra_polygon)

include_directories(.)
include_directories(./inc/)
include_directories(./src/)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TARGET tests)

set(INC_DIR inc)
set(SRC_DIR src)

set(SRC_EXT .cpp)
set(INC_EXT .hpp)

list(APPEND FILES dijkstra_polygon dijkstra_polygon_to_string test_data_reader test_util)

set(INC ${FILES})
list(TRANSFORM INC PREPEND ${INC_DIR}/)
list(TRANSFORM INC APPEND  ${INC_EXT})

set(SRC ${FILES})
list(TRANSFORM SRC PREPEND ${SRC_DIR}/)
list(TRANSFORM SRC APPEND  ${SRC_EXT})

add_executable(${TARGET} ${SRC_DIR}/${TARGET}${SRC_EXT} ${INC} ${SRC})

target_include_directories(${TARGET} PRIVATE ${INC_DIR})

# add dijkstra_polygon_to_string to LIB_SRC to build a library with to_string functionality
set(LIB_SRC dijkstra_polygon)
list(TRANSFORM LIB_SRC PREPEND ${SRC_DIR}/)
list(TRANSFORM LIB_SRC APPEND  ${SRC_EXT})
add_library(dijkstrapolygon SHARED ${LIB_SRC})