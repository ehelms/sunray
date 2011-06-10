#ifndef _WIREFRAME_H_
#define _WIREFRAME_H_

#include <string>

#include "vtk3DSImporter.h"

#include "surface.h"
#include "kdtree.h"
#include "simulation/simulationresultdata.h"

class vtkActor;

namespace GFX
{
	class Ray;
	class Vertex;

	class Wireframe : public Surface, public KdTree
	{
		public:
			Wireframe ( const std::string );
			Wireframe ( ); 

			virtual ~Wireframe();
			
			Simulation::SimulationResultData getResultData ( );

			void importGts ( const std::string );
			void import3ds ( const std::string );

			void trace ( const point3d, const vector3d, Ray & ) const;

		protected:
			void trace ( Ray &, const vector3d &, const point3d &, const point3d & ) const;
			void digestVtk3dsMesh ( vtk3DSMesh * );
			
		private:
			Simulation::SimulationResultData data;
	};

}

#endif // _WIREFRAME_H_

