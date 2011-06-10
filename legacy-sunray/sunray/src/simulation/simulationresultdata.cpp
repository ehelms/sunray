#include "simulationresultdata.h"

namespace Simulation
{

SimulationResultData::SimulationResultData()
{
}

SimulationResultData::~SimulationResultData()
{
}

void SimulationResultData::clear ( )
{
	data.clear();
}
void SimulationResultData::setVertexData ( unsigned int i, VertexResult v )
{
	data[i] = v;
}

VertexResult SimulationResultData::getVertexData ( unsigned int i )
{
	return data[i];
}



}
