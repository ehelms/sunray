/**
 * SunrayScenarioPane - gui pane for user input into the simulation
 *
 * @copyright 2007
 * @author Josiah Gore
 *
 * @package gui
 */

#ifndef _SUNRAY_SCENARIO_PANE_H_
#define _SUNRAY_SCENARIO_PANE_H_

#include <iostream>

#include "timeframe.h"
#include "granularityframe.h"
#include "conditionsframe.h"
#include "optionsframe.h"
#include "positionframe.h"
#include "../scenariopane.h"
#include "../simulation/solarsimulation.h"
#include "../simulation/solarscenario.h"

#include <gtkmm.h>


class SunrayScenarioPane : public ScenarioPane
{
	public:
		SunrayScenarioPane ( );
		virtual ~SunrayScenarioPane ( );

		Simulation::Simulation &getSimulation ( ) {
			return simulation;
		}

		virtual Simulation::Scenario *createScenario ( );

	protected:
		virtual void on_click_start ( );

		Simulation::SolarSimulation simulation;

		Gtk::VBox container;
		Gtk::HBox inputsContainer;
		Gtk::VBox leftColumn;
		Gtk::VBox rightColumn;
		TimeFrame chronos;
		GranularityFrame granul;
		ConditionsFrame atmos;
		OptionsFrame options;
		PositionFrame position;
		Gtk::Button startButton;
		Gtk::ProgressBar progressBar;

	private:
		Simulation::SolarScenario *scenario;
};

#endif // _SUNRAY_SCENARIO_PANE_H_

