/**
 * Actor - an object in the scenario that has some contribution to
 * the simulation aside from being a void from which no radiation
 * can escape.
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

#ifndef _ACTOR_H_
#define _ACTOR_H_


class SolarRay;

class Actor
{

	public:
		virtual ~Actor ( ) { }

		/**
		 * Determine whether SolarRay intersects this object
		 * @see Obstruction::intersects
		 */
		bool intersects ( const SolarRay & ) const;

		/**
		 * An actor can have an effect on the source ray.
		 * For example, a reflected SolarRay would be replaced with one
		 * starting at the point of contact. A transparent surface
		 * will leave an impression on the SolarRay to be used in the
		 * eventual accumulation amount
		 *
		 * @param source ray
		 * @param <output> resultant ray
		 * @return true if intersection occurs
		 */
		virtual bool intersects ( const SolarRay &, SolarRay & ) const = 0;
};


#endif // _ACTOR_H_


