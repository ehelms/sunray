#ifndef _WIREFRAME_VIEWER_H_
#define _WIREFRAME_VIEWER_H_

#include "vtkwidget.h"

#include <map>
#include <string>

#include "wireframeviewercontroller.h"
#include "gfx/wireframe.h"

class vtkActor;

namespace Visualization
{
	class Patch;

	class WireframeViewer : public VtkWidget
	{
		public:
			WireframeViewer ( );
			virtual ~WireframeViewer ( );

			void run ( );

			void setWireframe ( GFX::Wireframe & );
			void setDataResults ( Simulation::SimulationResultData & );
			
			Patch *getTotalPatch ( ) const;
			Patch *setCurrentPatch ( std::string );
			Patch *getCurrentPatch ( );
			bool hasPatch ( std::string );

			Patch *createPatch ( std::string );
			void deletePatch ( std::string );

		protected:
			GFX::Wireframe *wireframe;
			Simulation::SimulationResultData *data;
			std::vector<GFX::Face *> faces;
			std::vector<GFX::Vertex *> vertices;

			Patch *totalPatch;
			Patch *currentPatch;
			std::map<std::string, Patch *> patches;
			vtkActor *actor;

			void shadePatch ( );
			vtkActor *getActor ( );
	};

}

#endif // _WIREFRAME_VIEWER_H_

