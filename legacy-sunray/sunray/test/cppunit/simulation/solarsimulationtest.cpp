#include "solarsimulationtest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SolarSimulationTest);

void SolarSimulationTest::setUp ( )
{ }

void SolarSimulationTest::tearDown ( )
{ }

void SolarSimulationTest::testRunSimulation ( )
{
}

/*
class MockSolarSimulationTestInitializeScenario : public MockSolarSimulation {
	public:
		MockSolarSimulationTestInitializeScenario ( ScenarioCreator & s ) : MockSolarSimulation(s) {
			testScenario = NULL;
			processScenarioCalls = 0;
		}
		void setScenario ( SolarScenario * s ) { testScenario = s; }
		SolarScenario * getSolarScenario ( ) { 
			return testScenario;
		}
		void processScenario ( const SolarScenario & s ) {
			++processScenarioCalls;
		}
		void resetProcessScenarioCalls ( ) {
			processScenarioCalls = 0;
		}
		int getProcessScenarioCalls ( ) {
			return processScenarioCalls;
		}
	private:
		SolarScenario *testScenario;
		unsigned int processScenarioCalls;
};

void SolarSimulationTest::testInitializeScenario ( )
{
	MockSolarSimulationTestInitializeScenario sim(scenarioCreator);
	sim.setScenario(NULL);
	try {
		sim.initializeScenario();
		CPPUNIT_ASSERT(false);
	} catch ( SimulationInvalidScenarioException &e ) {
		CPPUNIT_ASSERT(true);
		CPPUNIT_ASSERT(sim.getProcessScenarioCalls() == 0);
	} catch ( std::exception &e ) {
		CPPUNIT_ASSERT(false);
	}

	SolarScenario *s = new SolarScenario;
	sim.setScenario(s);
	sim.resetProcessScenarioCalls();
	SolarScenario *result = sim.initializeScenario();
	CPPUNIT_ASSERT(true);
	CPPUNIT_ASSERT(result == s);
	CPPUNIT_ASSERT(sim.getProcessScenarioCalls() == 1);
	delete s;
}
	*/

class MockScenarioCreatorTestGetScenario : public ScenarioCreator {
	public:
		Scenario *scenario;
		void setScenario ( Scenario * s ) {
			scenario = s;
		}
		Scenario *createScenario ( ) { return scenario; }
};


void SolarSimulationTest::testGetScenario ( )
{
	MockScenarioCreatorTestGetScenario sc;
	MockSolarSimulation sim(sc);

	sc.setScenario(NULL);
	CPPUNIT_ASSERT(sim.getScenario() == NULL);
	
	MockScenario s;
	sc.setScenario(&s);
	CPPUNIT_ASSERT(sim.getScenario() == &s);

}

class MockSolarSimulationTestGetSolarScenario : public MockSolarSimulation {
	public:
		MockSolarSimulationTestGetSolarScenario ( ScenarioCreator & s ) : MockSolarSimulation(s) {
			testScenario = NULL;
		}
		void setScenario ( Scenario * s ) { testScenario = s; }
		Scenario * getScenario ( ) { 
			return testScenario;
		}
	private:
		Scenario *testScenario;
};

void SolarSimulationTest::testGetSolarScenario ( )
{
	MockSolarSimulationTestGetSolarScenario sim(scenarioCreator);
	
	sim.setScenario(NULL);
	try {
		sim.getSolarScenario();
		CPPUNIT_ASSERT(false);
	} catch ( SimulationInvalidScenarioException &e ) {
		CPPUNIT_ASSERT(true);
	} catch ( std::exception &e ) {
		CPPUNIT_ASSERT(false);
	}
	
	MockScenario s;
	sim.setScenario(&s);
	try {
		sim.getSolarScenario();
		CPPUNIT_ASSERT(false);
	} catch ( SimulationInvalidScenarioException &e ) {
		CPPUNIT_ASSERT(true);
	} catch ( std::exception &e ) {
		CPPUNIT_ASSERT(false);
	}
	
	SolarScenario ss;
	sim.setScenario(&ss);
	CPPUNIT_ASSERT(sim.getSolarScenario() == &ss);
}

