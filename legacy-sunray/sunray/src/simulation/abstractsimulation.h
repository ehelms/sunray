/**
 * AbstractSimulation - abstract base class for simulations with progress.
 *
 * @package simulation
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

#ifndef _ABSTRACT_SIMULATION_H_
#define _ABSTRACT_SIMULATION_H_


class AbstractSimulation
{

	public:
		/**
		 * Begin executing the simulation.
		 * TODO what should happen when a simulation begins?
		 */
		virtual void runSimulation ( ) = 0;

		/**
		 * Get the progress of the simulation as best it knows.
		 * Note this is not a percentage, but goes from 0 to 1.
		 *
		 * @return fraction current completed (1 => finished)
		 */
		virtual double getProgress ( ) const = 0;

		virtual ~AbstractSimulation ( ) { }

};


#endif // _ABSTRACT_SIMULATION_H_

