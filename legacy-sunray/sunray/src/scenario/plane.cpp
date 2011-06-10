/**
 * Plane - brief description of functionality
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

#include "plane.h"
#include "solarray.h"

#include "../gfx/primitives.h"

Plane::Plane ( )
{
}

Plane::~Plane ( ) { }


bool Plane::intersects ( const SolarRay &source, SolarRay & ) const
{
//	GFX::point3d origin;
//	origin.x = origin.y = origin.z = 0;
//	GFX::vector3d normal;
//	origin.x = 0; origin.y = 1; origin.z = 0;
//
//	GFX::point3d sourceOrigin = source.getOrigin();
//	GFX::vector3d dir = source.getDirection();
//
//	/* http://glasnost.itcarlow.ie/~powerk/Graphics/Notes/node12.html#SECTION001240000000000000000
//	 * r = D - 2( D * N) N
//	 */
//	GFX::point3d result = dir - 2 * ( dir * normal ) * normal;
//
//	/* http://www.cl.cam.ac.uk/teaching/2000/AGraphHCI/SMEG/node2.html#SECTION00023500000000000000
//	 * t = N * ( Q - E )
//	 *     -------------
//	 *        N * D
//	 */
	bool retr = false;
//	if ( normal * dir == 0 ) {
//		// parallel, no intersection
//		retr = false;
//	} else {
//		double t = ( normal * (origin - sourceOrigin) ) / ( normal * dir );
//		retr = ( t >= 0 );
//	}

	return retr;
}

