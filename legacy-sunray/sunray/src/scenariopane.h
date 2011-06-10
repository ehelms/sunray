/**
 * ScenarioPane - gui pane that can serve as scenario input to a simulation
 *
 * @copyright 2007
 * @author Josiah Gore
 *
 * @package gui
 */

#ifndef _SCENARIO_PANE_H_
#define _SCENARIO_PANE_H_

#include "simulation/scenariocreator.h"
#include "simulation/simulation.h"
#include "simulation/simulationcontroller.h"
#include "simulation/simulationresult.h"


#include <gtkmm/layout.h>

class ScenarioPane : public Gtk::Layout, public Simulation::ScenarioCreator
{

	public:
		ScenarioPane ( ) : Gtk::Layout() { }
		virtual ~ScenarioPane ( ) { }

		/**
		 * Note: this should never actually be returning a Simulation
		 */
		virtual Simulation::Simulation &getSimulation ( ) = 0;


	protected:
		virtual void on_click_start ( ) {
			startSimulation();
		}

		virtual void startSimulation ( ) {
			Simulation::SimulationController *simContr = Simulation::SimulationController::getInstance();
			simContr->spawnSimulation(&getSimulation());
		}

};


#endif // _SCENARIO_PANE_H_

