#ifndef SIMULATIONRESULTDATA_H_
#define SIMULATIONRESULTDATA_H_

#include <map>

#include "vertexresult.h"

namespace Simulation
{

class SimulationResultData
{
public:
	SimulationResultData();
	virtual ~SimulationResultData();
	
	void clear ( );
	void setVertexData ( unsigned int, VertexResult );
	VertexResult getVertexData ( unsigned int );
	
private:
	std::map<const unsigned int, VertexResult> data;
};

}

#endif /*SIMULATIONRESULTDATA_H_*/
