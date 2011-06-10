
include (config.cmake)
include (vtk.cmake)
include (includes.cmake)

ADD_SUBDIRECTORY (http/)
ADD_SUBDIRECTORY (simulation/)
ADD_SUBDIRECTORY (scenario/)
ADD_SUBDIRECTORY (visualization/)
ADD_SUBDIRECTORY (gfx/)

IF (VTK_FOUND)
	add_library (sr logger.cpp sunraygui.cpp
		sunrayscenariopane/sunrayscenariopane.cpp sunrayscenariopane/timeentry.cpp sunrayscenariopane/timeframe.cpp 
		sunrayscenariopane/granularityframe.cpp sunrayscenariopane/conditionsframe.cpp sunrayscenariopane/optionsframe.cpp
		sunrayscenariopane/positionframe.cpp)
	add_executable (sunray main.cpp)
	target_link_libraries (sunray sr sim scen visualize glib-2.0 gtkmm-2.4 gtkglextmm-x11-1.2 rt boost_filesystem boost_thread curl)
ENDIF (VTK_FOUND)
