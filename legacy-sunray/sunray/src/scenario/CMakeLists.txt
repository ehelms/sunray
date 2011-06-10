PROJECT (Sunray)

include_directories (${PROJECT_ROOT})
aux_source_directory (. SCEN)
add_library (scen ${SCEN})

