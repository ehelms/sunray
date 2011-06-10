#ifndef _EDGE_WATCH_H_
#define _EDGE_WATCH_H_


#include "edge.h"

#include <iostream>
#include <vector>

/**
 * Helper class for EdgeWatch
 */
class Vert2 
{

public:
	Vert2(int i, GFX::Edge *b) : i2(i) {	e = b ; }
	~Vert2() { }

	int getI() { return i2 ; }
	GFX::Edge *getE() { return e ; }


private:
	GFX::Edge *e ;
	int i2 ;
} ;

/**
 * Helper class for EdgeWatch
 */
class Vert1 
{

public:
	Vert1(int i) : i1(i) { }
	~Vert1() { }

	int getI() { return i1 ; }

	GFX::Edge *edge(int i2, GFX::Edge *e) {
		std::vector<Vert2>::iterator i ;
		for (i = v2s.begin() ; i != v2s.end() ; i++) {
			if (i2 == (*i).getI()) {
				return (*i).getE() ;
			}
		}
		if (i == v2s.end()) {
			Vert2 v(i2, e) ;
			v2s.push_back(v) ;
			return e ;
		}
	}

private:
	std::vector<Vert2> v2s ;
	int i1 ;
} ;

/**
 * EdgeWatch class. Tracks edges to ensure there are no duplicates when creating a surface
 */
class EdgeWatch
{
public:
	/**
	 * null constructor
	 */
	EdgeWatch() { }
	/**
	 * destructor
	 */
	~EdgeWatch() { }

	/**
	 * Insert an edge if it does not exist, and return the edge
	 */
	GFX::Edge *edge(int i1, int i2, GFX::Edge *e) {
		
		// we need to establish a rule: i1 is the lower number
		if (i2 > i1) {
			int tmp = i1 ;
			i1 = i2 ;
			i2 = tmp ;
		}

		std::vector<Vert1>::iterator i ;
		for (i = v1s.begin() ; i != v1s.end() ; i++) {
			if (i1 == (*i).getI()) {
				return (*i).edge(i2, e) ;
			}
		}
		if (i == v1s.end()) {
			Vert1 v(i1) ;
			GFX::Edge *b = v.edge(i2, e) ;
			v1s.push_back(v) ;
			return b ;
		}

	}

private:
	std::vector<Vert1> v1s ;

} ;


#endif
