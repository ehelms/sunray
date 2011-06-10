#include "vertexresult.h"

#include <iostream>

namespace Simulation
{

VertexResult::VertexResult()
{
}

VertexResult::~VertexResult()
{
}

void VertexResult::clear ( )
{
	// todo replace 27 with band enum or const
	for ( size_t i = 0; i < 27; ++i ) {
		clear(i);
	}
}

void VertexResult::clear ( unsigned int i )
{
	irradiance[i] = 0;
}

void VertexResult::accumulate ( std::map<const unsigned int, double> irrad )
{
	std::map<const unsigned int, double>::iterator i;
	for ( i = irrad.begin(); i != irrad.end(); ++i ) {
		irradiance[(*i).first] += (*i).second;
	}
}

void VertexResult::normalizeIrradiance ( double factor )
{
	std::map<const unsigned int, double>::iterator d;
	for ( d = irradiance.begin(); d != irradiance.end(); ++d ) {
		(*d).second /= factor;
	}
}

std::map<const unsigned int, double> VertexResult::getIrradiance ( )
{
	return irradiance;
}

}
