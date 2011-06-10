project (Sunray)

set (PROJECT_ROOT ${PROJECT_SOURCE_DIR})
set (SRC_ROOT ${PROJECT_ROOT}/src)
set (BIN_ROOT ${PROJECT_ROOT}/bin)
set (LIB_ROOT ${PROJECT_ROOT}/lib)
set (TEST_ROOT ${PROJECT_ROOT}/test)

set (EXECUTABLE_OUTPUT_PATH ${BIN_ROOT})
set (LIBRARY_OUTPUT_PATH ${LIB_ROOT})
set (CMAKE_CXX_FLAGS "-Wall -Wno-deprecated -g")

include_directories (${SRC_ROOT})

add_subdirectory (${SRC_ROOT})
add_subdirectory (${TEST_ROOT}/fit/fixtures)
add_subdirectory (${TEST_ROOT}/cppunit)
