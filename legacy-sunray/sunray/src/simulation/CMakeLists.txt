PROJECT (Sunray)

INCLUDE_DIRECTORIES (${PROJECT_ROOT})
LINK_DIRECTORIES (${PROJECT_ROOT}/gfx)

ADD_CUSTOM_COMMAND (
	OUTPUT specrad1.o
	COMMAND g95 -ftrace=full -o ${CMAKE_CURRENT_SOURCE_DIR}/specrad1.o -c ${CMAKE_CURRENT_SOURCE_DIR}/specrad1.f
)
aux_source_directory (. SIM.SRC)
add_library (sim ${SIM.SRC} specrad1.o)

TARGET_LINK_LIBRARIES (sim gfx f95)
