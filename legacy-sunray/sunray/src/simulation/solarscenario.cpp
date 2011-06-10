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
 
#include "solarscenario.h"

namespace Simulation
{

	SolarScenario::SolarScenario ( ) { }
	SolarScenario::~SolarScenario ( ) { }

	void SolarScenario::setParameters ( std::string bt,  std::string et,  unsigned int d,  unsigned int m,  unsigned int y,  double ozo,  double vis,  std::string wf, double lat, double lng, unsigned char hlat, unsigned char hlong, unsigned int orien, unsigned int elev, unsigned int ga,  unsigned int gz,  unsigned int gt )
	{
		int beginMinute, beginHour, endMinute, endHour;
		beginMinute = std::atoi(bt.substr(bt.length()-2).c_str());
		beginHour = std::atoi(bt.substr(0, bt.length()-2).c_str());
		endMinute = std::atoi(et.substr(et.length()-2).c_str());
		endHour = std::atoi(et.substr(0, et.length()-2).c_str());

		ground.setOrientation(orien);
		
		parameters.setParameters(beginMinute, beginHour, endMinute, endHour, d, m, y, ozo, vis, wf, lat, lng, hlat, hlong, orien, elev, ga, gz, gt);
	}

	void SolarScenario::setModelFilename ( std::string file ) {
		filename = file;
	}

	void SolarScenario::setDirect ( bool dir ) {
		direct = dir;
	}

	void SolarScenario::setDiffuse ( bool dif ) {
		diffuse = dif;
	}

	void SolarScenario::setReflective ( bool refl ) {
		reflective = refl;
	}

}
