/**
 * SunrayScenarioPane - gui pane for user input into the simulation
 *
 * @copyright 2007
 * @author Josiah Gore
 *
 * @package gui
 */

#include "sunrayscenariopane.h"

#include "../simulation/solarscenario.h"
#include "../simulation/simulationcontroller.h"



#include <iostream>
#include <string>
#include <sstream>


SunrayScenarioPane::SunrayScenarioPane ( ) :
	ScenarioPane(),
	simulation(dynamic_cast<ScenarioCreator &>(*this)),
	container(),
	inputsContainer(true, 5),
	leftColumn(),
	rightColumn(),
	chronos("Time & Date"),
	granul("Calculation Granularity"),
	atmos("Atmospheric Conditions"),
	options("Simulation Options"),
	position("Location & Orientation"),
	startButton("Run Simulation"),
	progressBar()
{
	add(container);
	container.add(inputsContainer);
	inputsContainer.set_border_width(10);

	inputsContainer.add(leftColumn);
	inputsContainer.add(rightColumn);

	leftColumn.pack_start(chronos);
	leftColumn.pack_start(granul);

	rightColumn.pack_start(atmos);
	rightColumn.pack_start(options);
	rightColumn.pack_start(position);
	
	container.add(startButton);
	startButton.set_border_width(25);
	// TODO: start button should trigger a SimulationController to start
	startButton.signal_clicked().connect(sigc::mem_fun(this, &SunrayScenarioPane::on_click_start));

	show_all();


	scenario = new Simulation::SolarScenario;
}

SunrayScenarioPane::~SunrayScenarioPane ( ) 
{
	delete scenario;
}

Simulation::Scenario *SunrayScenarioPane::createScenario ( )
{
	std::string beginTime, endTime;
	unsigned int day, month, year;
	chronos.getAllInputs(beginTime, endTime, day, month, year);

	unsigned int granulTime, granulAzimuth, granulZenith;
	granul.getAllInputs(granulTime, granulAzimuth, granulZenith);

	double ozone, aod;
	atmos.getAllInputs(ozone, aod);

	bool direct, diffuse, reflective;
	std::string weightingFunc, modelFilename;
	options.getAllInputs(direct, diffuse, reflective, weightingFunc, modelFilename);

	double latitude, longitude;
	unsigned char latHemisphere, longHemisphere;
	unsigned int orientation, elevation;
	position.getAllInputs(latitude, latHemisphere, longitude, longHemisphere, orientation, elevation);


	scenario->setParameters(
			beginTime, endTime, day, month, year,
			ozone, aod, weightingFunc,
			latitude, longitude, latHemisphere, longHemisphere, orientation, elevation,
			granulAzimuth, granulZenith, granulTime
	); 
	scenario->setModelFilename(modelFilename);
	scenario->setDirect(direct);
	scenario->setDiffuse(diffuse);
	scenario->setReflective(reflective);

	return scenario;
}

void SunrayScenarioPane::on_click_start ( )
{
	container.remove(startButton);
	//container.add(progressBar);
	progressBar.set_text("Initializing...");
	progressBar.set_fraction(0.);

	ScenarioPane::on_click_start();
}


