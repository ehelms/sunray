#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <string>

#include <sigc++/signal.h>

#include "abstractsimulation.h"
#include "scenariocreator.h"
#include "parameters.h"
#include "simulationresult.h"

#include "StringTools.h"


namespace Simulation
{

	/**
	 * Exceptions
	 */
	class SimulationException : public virtual std::exception { };
	class SimulationInvalidParameterException : public virtual SimulationException { };
	class SimulationInvalidResolutionException : public virtual SimulationException { };
	class SimulationInvalidScenarioException : public virtual SimulationException { };

	/**
	 * Resources
	 */
	const double PI = 3.141592653589793;
	
	const std::string RESOURCE_PATH = "../src/simulation/resources/";


	typedef sigc::signal<void> onCompleteEvent;

	class Executor { };

	class Simulation : public AbstractSimulation
	{
		public:
			/** @see AbstractSimulation::runSimulation() */
			virtual void runSimulation ( ) { }
			/** @see AbstractSimulation::getProgress() */
			virtual double getProgress ( ) const { return 1.0; }

			virtual SimulationResult &getResult ( ) { }

			onCompleteEvent signal_complete ( ) {
				return completeEvent;
			}

		protected:
			onCompleteEvent completeEvent;
			void fireCompleteEvent ( ) {
				completeEvent.emit();
			}

			ScenarioCreator &getScenarioCreator ( ) {
				return scenarioCreator;
			}
			void setScenarioCreator ( ScenarioCreator &sc ) {
				scenarioCreator = sc;
			}

			Executor &getExecutor ( ) {
				return executor;
			}
			void setExecutor ( Executor &ex ) {
				executor = ex;
			}

			Simulation ( ScenarioCreator &sc ) : scenarioCreator(sc) { }
			virtual ~Simulation ( ) { }

		private:
			ScenarioCreator &scenarioCreator;
			Executor executor;

	};

}

#endif // _SIMULATION_H_

