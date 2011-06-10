#include "reflectivegroundtest.h"

#include "scenario/reflectiveground.h"
#include "simulation/solarscenario.h"

ReflectiveGroundTest::ReflectiveGroundTest ( ) {
	PUBLISH(ReflectiveGroundTest, int, phiForOrientation);
	PUBLISH(ReflectiveGroundTest, int, figureDirection);
	PUBLISH(ReflectiveGroundTest, int, orientation);
}

int ReflectiveGroundTest::phiForOrientation ( ) {
	Simulation::SolarScenario *scen = new Simulation::SolarScenario;
	scen->setModelFilename("/home/josiah/workspace/sunray/trunk/test/src/models/TestCube.gts");
	scen->setParameters("","",0,0,0,0,0,"",0,0,0,0, figureDirection, 0,0,0,0);
	
	Simulation::ReflectiveGround ground = scen->getReflectiveGround();
	int retr = ground.getPhiForOrientation(orientation);
	
	return retr;
}

int ReflectiveGroundTest::numberOfZones ( ) {
	Simulation::SolarScenario *scen = new Simulation::SolarScenario;
	scen->setModelFilename("/home/josiah/workspace/sunray/trunk/test/src/models/TestCube.gts");
	
	Simulation::ReflectiveGround ground = scen->getReflectiveGround();
	int retr = ground.getNumberOfZones();
	
	return retr;
}
