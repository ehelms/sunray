/**
 * Scenario - abstract description of a runnable simulation
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

#ifndef _SCENARIO_H_
#define _SCENARIO_H_


namespace Simulation
{
	class Parameters;

	class Scenario
	{
		public:
			/**
			 * Returns whether this scenario has valid parameters and all
			 * necessary resources available.
			 *
			 * @return whether scenario is valid
			 */
			virtual bool isValid ( ) = 0;

			/**
			 * Get the scenario parameters defined for this simulation
			 *
			 * @return Parameters
			 */
			virtual const Simulation::Parameters &getParameters ( ) const = 0;

			virtual ~Scenario ( ) { }
	};

}

#endif // _SCENARIO_H_

