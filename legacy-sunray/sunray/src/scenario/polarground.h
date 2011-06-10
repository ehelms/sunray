/**
 * PolarGround - brief description of functionality
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

#ifndef _POLAR_GROUND_H_
#define _POLAR_GROUND_H_

#include "reflectiveground.h"

namespace Simulation
{

	class PolarGround : protected ReflectiveGround
	{

		public:
			PolarGround ( );
			virtual ~PolarGround ( );

			/**
			 * Set the rho resolution relative to model height
			 *
			 * @param height of target model
			 * @param ratio to height
			 */
			void setRhoResolution ( double, double );

			/**
			 * Set phi resolution of inner circle in degrees. The
			 * intermediate zone uses twice this (half the polygons).
			 * The outer zone is an undivided polygon extending to infinity.
			 *
			 * @param phi resolution (degrees)
			 */
			void setPhiResolution ( double );

		private:
			double phiResolution, rhoResolution;

	};

}

#endif // _POLAR_GROUND_H_


