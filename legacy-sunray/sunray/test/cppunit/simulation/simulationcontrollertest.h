/**
 * Automated test for simulation controller
 */

#ifndef _SIMULATION_CONTROLLER_TEST_H_
#define _SIMULATION_CONTROLLER_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "simulation/simulationcontroller.h"


class SimulationControllerTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(SimulationControllerTest);
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp ( ) { }
		void tearDown ( ) { }

	protected:

	private:
		SimulationController contr;

};

CPPUNIT_TEST_SUITE_REGISTRATION(SimulationControllerTest);

#endif // _SIMULATION_CONTROLLER_TEST_H_

