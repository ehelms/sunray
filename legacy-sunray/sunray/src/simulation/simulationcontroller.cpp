
#include "simulationcontroller.h"

#include <sigc++/bind.h>
#include <boost/thread/thread.hpp>

namespace Simulation
{

	SimulationController *SimulationController::instance = 0;

	SimulationController *SimulationController::getInstance ( )
	{
		if ( instance == 0 ) {
			instance = new SimulationController;
		}

		return instance;
	}


	unsigned int SimulationController::spawnSimulation ( Simulation *sim )
	{
		std::cerr << "TODO: create a thread for running simulations" << std::endl;
		unsigned int pos = simulations.size();
		simulations.push_back(sim);

		// connect to complete signal and spawn a thread
		sim->signal_complete().connect(sigc::bind(sigc::mem_fun(this, &SimulationController::fireSimulationCompleteEvent), pos));
		sim->runSimulation();

		//boost::thread simThread(sigc::mem_fun(sim, &Simulation::runSimulation));
		//
		return pos;
	}

	double SimulationController::getProgress ( unsigned int index )
	{
		if ( index > simulations.size() ) {
			throw SimulationInvalidParameterException();
		}

		return simulations[index]->getProgress();
	}

	Simulation *SimulationController::getSimulation ( unsigned int index )
	{
		if ( index < 0 || index > simulations.size() ) {
			throw SimulationInvalidParameterException();
		}

		return simulations[index];
	}

	void SimulationController::fireSimulationCompleteEvent ( unsigned int simIndex )
	{
		completeEvent.emit(simIndex);
	}



	SimulationController::SimulationController ( )
	{ }

	SimulationController::~SimulationController ( )
	{ }


}

