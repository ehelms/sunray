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

#ifndef _SUNRAYGUI_H_
#define _SUNRAYGUI_H_

#include <gtkmm.h>

#include "sunrayscenariopane/sunrayscenariopane.h"
#include "visualization/wireframeviewer.h"
#include "resultspane.h"

class Simulation::SimulationController;

class SunrayGui : public Gtk::Window {

	public:
		/**
		 * Instantiates child widgets and displays everything
		 */
		SunrayGui ( );
		virtual ~SunrayGui ( );

		/**
		 * Displays results panes for the indicated simulation. The simulation
		 * is indicated with an index into the Simulation Controller's pool.
		 *
		 * Used as a callback connected to the Simulation Controller's 
		 * signal_simulation_complete() event.
		 *
		 * @param index into simulation pool
		 */
		void displayResult ( unsigned int );

	protected:
		/**
		 * Gets instance of the Simulation Controller
		 *
		 * @return pointer to Simulation Controller
		 */
		Simulation::SimulationController *getSimulationController ( );

		Gtk::Notebook notebook;
		SunrayScenarioPane scenarioPane;
		Visualization::WireframeViewer visualizationPane;
		ResultsPane resultsPane;

	private:
		void buildInterface ( );

};

#endif // _SUNRAYGUI_H_
