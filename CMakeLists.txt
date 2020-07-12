cmake_minimum_required(VERSION 3.16)
project(dijkstra_polygon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(TARGET tests)

set(INC_DIR inc)
set(SRC_DIR src)

set(SRC_EXT .cpp)
set(INC_EXT .hpp)

list(APPEND LIB dijkstra_polygon dijkstra_polygon_to_string test_data_reader test_util)

set(INC ${LIB})
list(TRANSFORM INC PREPEND ${INC_DIR}/)
list(TRANSFORM INC APPEND  ${INC_EXT})

set(SRC ${LIB})
list(TRANSFORM SRC PREPEND ${SRC_DIR}/)
list(TRANSFORM SRC APPEND  ${SRC_EXT})

add_executable(${TARGET} ${SRC_DIR}/${TARGET}${SRC_EXT} ${INC} ${SRC})

target_include_directories(${TARGET} PRIVATE ${INC_DIR})