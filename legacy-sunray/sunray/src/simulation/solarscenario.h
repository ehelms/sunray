/**
 * SolarScenario - parameters for simulation of solar exposure
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

#ifndef _SOLAR_SCENARIO_H_
#define _SOLAR_SCENARIO_H_

#include <string>

#include "parameters.h"
#include "scenario.h"
#include "scenario/reflectiveground.h"

namespace Simulation
{

	class SolarScenario : public Scenario
	{
		public:
			SolarScenario ( );
			~SolarScenario ( );

			/**
			 * Convenience function that allows all parameters to be set at once
			 *
			 * @param begin time
			 * @param end time
			 * @param day
			 * @param month
			 * @param year
			 * @param ozone
			 * @param aerosol optical depth
			 * @param spectral weighting function name
			 * @param latitude
			 * @param longitude
			 * @param latitudinal hemisphere
			 * @param longitudinal hemisphere
			 * @param orientation relative to north
			 * @param elevation
			 * @param azimuthal granularity
			 * @param zenith granularity
			 * @param time granularity
			 */
			void setParameters ( std::string, std::string, unsigned int, unsigned int, unsigned int, double, double, std::string, double, double, unsigned char, unsigned char, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int );
			const Parameters &getParameters ( ) const {
				return parameters;
			}

			void setModelFilename ( std::string );
			std::string getModelFilename ( ) const {
				return filename;
			}
			void setDirect ( bool );
			bool getDirect ( ) const {
				return direct;
			}
			void setDiffuse ( bool );
			bool getDiffuse ( ) const {
				return diffuse;
			}
			void setReflective ( bool );
			bool getReflective ( ) const {
				return reflective;
			}

			const ReflectiveGround &getReflectiveGround ( ) const {
				return ground;
			}

			virtual bool isValid ( ) {
				return true;
			}

		protected:
			Parameters parameters;
			bool direct, diffuse, reflective;
			ReflectiveGround ground;
			std::string filename;
	};
}

#endif // _SOLAR_SCENARIO_H_

