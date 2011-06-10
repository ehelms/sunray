/**
 * SimulationController - handles multi-threaded simulation execution.
 * Maintains a pool of simulations, responsible for their creation and destruction.
 * Singleton.
 *
 * @copyright 2007
 * @author Josiah Gore
 *
 * @package simulation
 */

#ifndef _SIMULATION_CONTROLLER_H_
#define _SIMULATION_CONTROLLER_H_

#include "simulation.h"

#include <vector>


namespace Simulation
{

	typedef sigc::signal<void, unsigned int> simulationCompleteEvent;

	class SimulationController
	{
		public:
			/**
			 * Get the singleton instance of this controller
			 *
			 * @return singleton instance
			 */
			static SimulationController *getInstance ( );

			unsigned int spawnSimulation ( Simulation * );
			double getProgress ( unsigned int );
			Simulation *getSimulation ( unsigned int );

			simulationCompleteEvent signal_simulation_complete ( ) {
				return completeEvent;
			}

		protected:
			SimulationController ( );
			virtual ~SimulationController ( );

			std::vector<Simulation *> simulations;
			simulationCompleteEvent completeEvent;

		private:
			static SimulationController *instance;

			void fireSimulationCompleteEvent ( unsigned int );
	};

}

#endif // _SIMULATION_CONTROLLER_H_

