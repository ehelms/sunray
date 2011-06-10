project (Sunray)

include (${SRC_ROOT}/config.cmake)
include (${SRC_ROOT}/vtk.cmake)
include (${SRC_ROOT}/includes.cmake)

include_directories (${CPPUNIT_INCLUDE_DIR} ${SRC_ROOT})

aux_source_directory (simulation TESTS.SIM)
add_executable (suite.full main.cpp ${TESTS.SIM})
target_link_libraries (suite.full cppunit dl sim scen boost_filesystem sigc-2.0)
