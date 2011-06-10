PROJECT (Sunray)


ADD_LIBRARY (visualize wireframeviewer.cpp wireframeviewercontroller.cpp visualization.cpp patch.cpp vtkwidget.cpp)
TARGET_LINK_LIBRARIES (visualize vtkRendering gfx)

IF (BUILD_TESTS)
	ADD_EXECUTABLE(hemispheretest hemispheretest.cpp visualization.cpp) 
	TARGET_LINK_LIBRARIES(hemispheretest vtkRendering gfx)
ENDIF (BUILD_TESTS)
