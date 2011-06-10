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

#ifndef _REFLECTIVE_GROUND_H_
#define _REFLECTIVE_GROUND_H_

#include <vector>

#include "plane.h"
#include "polargridzone.h"

namespace Simulation
{

	class ReflectiveGround : public Plane
	{
		public:
			ReflectiveGround ( );
			virtual ~ReflectiveGround ( );
			
			void setOrientation ( int );
			void setPhiResolution ( int );
			
			int getPhiForOrientation ( int );
			int getNumberOfZones ( );
			
		protected:
			void initializeZones ( );
			
		private:
			int phi;
			int resolution;
			
			std::vector<PolarGridZone> zones;
	};
	

}

#endif // _REFLECTIVE_GROUND_H_


