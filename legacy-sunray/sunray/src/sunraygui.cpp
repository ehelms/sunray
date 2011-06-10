/**
 * SunrayGui - primary GTK window
 *
 * @package default
 *
 * @copyright 2007 Computational Photobiology Research Group
 * @author Josiah Gore
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * A copy of the GNU General Public License was provided along with 
 * this program, and is also available at <http://www.gnu.org/licenses/>.
 */

#include "sunraygui.h"

#include "simulation/simulationcontroller.h"
#include "simulation/simulationresult.h"


SunrayGui::SunrayGui ( ) :
	Gtk::Window(Gtk::WINDOW_TOPLEVEL),
	notebook(),
	scenarioPane(),
	visualizationPane(),
	resultsPane()
{
	set_title("Sunray");
	set_default_size(700, 600);
	move(100, 100);
	buildInterface();

	getSimulationController()->signal_simulation_complete().connect(sigc::mem_fun(this, &SunrayGui::displayResult));

	show_all();
}

SunrayGui::~SunrayGui ( ) { }

/** Creates a tabbed container and adds a ScenarioPane, 
 *  VisualizationPane, and ResultsPane to it. */
void SunrayGui::buildInterface ( )
{
	Gtk::VBox *box = new Gtk::VBox;
	add(*box);

	box->pack_start(notebook);
	notebook.set_border_width(10);
	notebook.append_page(scenarioPane, "Scenario");
	notebook.append_page(visualizationPane, "Visualization");
	notebook.append_page(resultsPane, "Data Results");
}

void SunrayGui::displayResult ( unsigned int index )
{
	Simulation::SimulationController *simContr = getSimulationController();
	Simulation::SimulationResult &sr = simContr->getSimulation(index)->getResult();

	visualizationPane.setWireframe(sr.getWireframe());
	//visualizationPane.setDataResults(sr.getResultData());
	visualizationPane.run();

	// whenever this is implemented
	//resultsPane.setResults(sr);
	//resultsPane.run();
}

Simulation::SimulationController *SunrayGui::getSimulationController ( )
{
	return Simulation::SimulationController::getInstance();
}

