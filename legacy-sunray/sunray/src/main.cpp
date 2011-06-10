/**
 * main.cpp - driver, loads configs and kickstarts the gui
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

#include <cstdio>
#include <iostream>
#include <exception>

#include "sunraygui.h"
//#include "sunrayscenariofile.h"

#include <gtkmm/main.h>
#include <gtkglmm.h>

int main ( int argc, char *argv[] )
{
	std::srand(std::time(NULL));

	// TODO if any user specific configurations should be loaded, do it now
	
	// TODO if -v display some version and configuration information here

	try {
		Gtk::Main kit(argc, argv);
		Gtk::GL::init(argc, argv);

		SunrayGui gui;
		kit.run(gui);
	} catch ( exception e ) {
		std::cerr << "Caught fatal exception at last moment. All is lost." << std::endl;
	}

	return 0;
}

