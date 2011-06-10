/**
 * ImportedActor - brief description of functionality
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

#ifndef _IMPORTED_ACTOR_H_
#define _IMPORTED_ACTOR_H_

#include "actor.h"
#include "../gfx/wireframe.h"

class SolarRay;

class ImportedActor : public Actor, public GFX::Wireframe
{

	public:
		ImportedActor ( );
		virtual ~ImportedActor ( );

		/**
		 * Determine whether ray intersects and provide resultant
		 * ray.
		 * @see Actor::intersects
		 */
		virtual bool intersects ( const SolarRay &, SolarRay & ) const;
};


#endif // _IMPORTED_ACTOR_H_


