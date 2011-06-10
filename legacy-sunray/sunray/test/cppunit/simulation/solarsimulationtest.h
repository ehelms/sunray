/**
 * Automated unit testing fixture for SolarSimulation
 */

#ifndef _SOLAR_SIMULATION_TEST_H_
#define _SOLAR_SIMULATION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "simulation/solarsimulation.h"
#include "simulation/scenariocreator.h"


using namespace Simulation;

class MockScenario : public Scenario {
	public:
		MockScenario ( ) : parameters() { }
		bool isValid ( ) { return true; }
		const Parameters &getParameters ( ) const { return parameters; }
	private:
		Parameters parameters;
};

class MockScenarioCreator : public ScenarioCreator {
	public:
		Scenario *createScenario ( ) { return new MockScenario; }
};

class MockSolarSimulation : public SolarSimulation {
	public:
		MockSolarSimulation(ScenarioCreator &sc) : SolarSimulation(sc) { }
		virtual Scenario * getScenario ( ) { 
			return SolarSimulation::getScenario(); 
		}
		virtual SolarScenario * getSolarScenario ( ) { 
			return SolarSimulation::getSolarScenario(); 
		}
};

class SolarSimulationTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(SolarSimulationTest);
	CPPUNIT_TEST(testRunSimulation);
	CPPUNIT_TEST(testGetScenario);
	CPPUNIT_TEST(testGetSolarScenario);
	CPPUNIT_TEST_SUITE_END();

	public:
		SolarSimulationTest ( ) : fixture(scenarioCreator) { }
		void setUp ( );
		void tearDown ( );

	protected:
		void testRunSimulation ( );
		void testGetScenario ( );
		void testGetSolarScenario ( );
		
	private:
		MockScenarioCreator scenarioCreator;
		MockSolarSimulation fixture;

};

#endif // _SOLAR_SIMULATION_TEST_H_
