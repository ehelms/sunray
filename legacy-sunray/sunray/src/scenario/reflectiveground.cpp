/**
 * ReflectiveGround - brief description of functionality
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

#include "reflectiveground.h"
#include "simulation/simulation.h"

namespace Simulation
{

	ReflectiveGround::ReflectiveGround ( )
	{
		phi = 0;
	}
	
	ReflectiveGround::~ReflectiveGround ( ) { }
	
	void ReflectiveGround::setOrientation ( int phi )
	{
		this->phi = phi;
	}
	
	int ReflectiveGround::getPhiForOrientation ( int orientation )
	{
		return phi + orientation;
	}
	
	void ReflectiveGround::setPhiResolution ( int resolution )
	{
		if ( 360 % resolution != 0 ) {
			throw SimulationInvalidResolutionException();
		}
		
		this->resolution = resolution;
		
		initializeZones();
	}
	
	int ReflectiveGround::getNumberOfZones ( )
	{
	}

	void ReflectiveGround::initializeZones ( )
	{
		zones.clear();
	}
}

