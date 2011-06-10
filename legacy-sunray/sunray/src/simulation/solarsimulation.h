#ifndef _SOLAR_SIMULATION_H_
#define _SOLAR_SIMULATION_H_

#include "../gfx/wireframe.h"

#include "../scenario/importedactor.h"

#include "simulation.h"
#include "solarscenario.h"
#include "parameters.h"
#include "radiativetransfermodel.h"


namespace Simulation
{
	class ReflectiveGround;

	class SolarSimulation : public Simulation
	{
		public:
			SolarSimulation ( ScenarioCreator & );
			virtual ~SolarSimulation ( );

			//void initScenario ( const Parameters );
			void runSimulation ( ) throw (SimulationInvalidScenarioException, SimulationException);
			double getProgress ( ) const;
			SimulationResult &getResult ( );

			GFX::Wireframe &getWireframe ( );

		protected:
			void traceDiffuse ( GFX::Vertex *, double, double );
			void traceDirect ( GFX::Vertex * );
			void traceReflective ( GFX::Vertex *, const ReflectiveGround & );

			void normalizeVertices ( std::vector<GFX::Vertex *>, double );

			virtual Scenario * getScenario ( );
			virtual ImportedActor getTargetActor ( );
			virtual SolarScenario * getSolarScenario ( );
			virtual void processScenario ( const SolarScenario & );
		private:
			RadiativeTransferModel *radTran;
			ImportedActor figure;
			Parameters input;

			double percentComplete;

			enum FILE_TYPE{T3DS,GTS,XML,NONE};
			static FILE_TYPE modelType(const char* filename) {
				boost::filesystem::path p(filename,boost::filesystem::native);
				std::string ext=boost::filesystem::extension(p);
				if(ext==".3DS"||ext==".3ds") {
					return T3DS;
				} else if(ext==".GTS"||ext==".gts") {
					return GTS;
				} else if(ext==".XML"||ext==".xml") {
					return XML;
				} else return NONE;
			}

	};

}

#endif // _SOLAR_SIMULATION_H_

