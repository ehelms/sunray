project (Sunray)

set (CPPFIT ${TEST_ROOT}/fit/cpp/CppTestTools)
include_directories (${PROJECT_ROOT}/src ${CPPFIT})
link_directories (${PROJECT_ROOT}/lib ${CPPFIT}/lib)

aux_source_directory (. FIXTURES)
add_executable (sunrayfitfixtures ${FIXTURES})
target_link_libraries (sunrayfitfixtures scen sim FitLib Concurrency)
