#ifndef _SIMULATION_RESULT_H_
#define _SIMULATION_RESULT_H_

#include "../gfx/wireframe.h"
#include "simulationresultdata.h"

namespace Simulation
{

	class SimulationResult
	{
		public:
			SimulationResult ( GFX::Wireframe &wf ) : wireframe(wf) { }
			~SimulationResult ( ) { }
			GFX::Wireframe &getWireframe ( ) { return wireframe; }
			SimulationResultData getResultData ( ) { return wireframe.getResultData(); }
			
		protected:
			GFX::Wireframe &wireframe;
	};

}

#endif // _SIMULATION_RESULT_H_

