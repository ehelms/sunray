PROJECT (Sunray)

ADD_LIBRARY (gfx color.cpp primitives.cpp wireframe.cpp kdtree.cpp bbox.cpp surface.cpp face.cpp edge.cpp vertex.cpp bsphere.cpp)
TARGET_LINK_LIBRARIES (gfx gts glib-2.0 vtkCommon vtkHybrid vtkGraphics vtkRendering vtkIO)
