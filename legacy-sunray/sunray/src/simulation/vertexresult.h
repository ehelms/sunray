#ifndef VERTEXRESULT_H_
#define VERTEXRESULT_H_

#include <map>

namespace Simulation
{

class VertexResult
{
public:
	VertexResult();
	virtual ~VertexResult();
	
	void clear ( );
	void clear ( unsigned int );
	void accumulate ( std::map<const unsigned int, double> );
	void normalizeIrradiance ( double );
	std::map<const unsigned int, double> getIrradiance ( );
		
private:
	std::map<const unsigned int, double> irradiance;
};

}

#endif /*VERTEXRESULT_H_*/
