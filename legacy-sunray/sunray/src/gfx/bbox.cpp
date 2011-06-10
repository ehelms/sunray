#include "bbox.h"

#include <iterator>

#include "face.h"
#include "vertex.h"
#include "Ray.h"
#include "../logger.h"

namespace GFX
{

	BBox::BBox ( Face *face )
	{
		this->face = face;

		std::vector<Vertex *> verts = face->getVertices();
		minPoint = minimumPoint(verts[0]->getPoint(), verts[1]->getPoint(), verts[2]->getPoint());
		maxPoint = maximumPoint(verts[0]->getPoint(), verts[1]->getPoint(), verts[2]->getPoint());
	}

	BBox::BBox ( int depth )
	{
		this->face = NULL;
		this->depth = depth;

		minPoint.x = minPoint.y = minPoint.z = 1e100;
		maxPoint.x = maxPoint.y = maxPoint.z = -1e100;
	}

	BBox::~BBox ( )
	{ }

	/* 
	   Fast Ray-Box Intersection
	   by Andrew Woo
	   from "Graphics Gems", Academic Press, 1990
	   */
	bool BBox::intersects ( const Ray ray ) const
	{
		if ( face && face->isCulled() ) {
			return false;
		}

		double minB[] = { minPoint.x, minPoint.y, minPoint.z };
		double maxB[] = { maxPoint.x, maxPoint.y, maxPoint.z };

		point3d org = ray.getOrigin();
		double origin[] = { org.x, org.y, org.z };

		vector3d dr = ray.getDirection();
		double dir[] = { dr.x, dr.y, dr.z };


		enum { RIGHT, LEFT, MIDDLE };
		int quadrant[3];
		double candidatePlane[3];
		bool inside = true;
		// Find candidate planes; this loop can be avoided if
		// rays cast all from the eye(assume perpsective view) 
		for ( int i = 0; i < 3; ++i ) {
			if ( origin[i] < minB[i] ) {
				quadrant[i] = LEFT;
				candidatePlane[i] = minB[i];
				inside = false;
			} else if ( origin[i] > maxB[i] ) {
				quadrant[i] = RIGHT;
				candidatePlane[i] = maxB[i];
				inside = false;
			} else {
				quadrant[i] = MIDDLE;
			}
		}

		if ( inside ) {
			return true;
		}

		double maxT[] = { 0., 0., 0. };
		// Calculate T distances to candidate planes
		for ( int i = 0; i < 3; ++i ) {
			if ( quadrant[i] != MIDDLE && dir[i] != 0. ) {
				maxT[i] = (candidatePlane[i] - origin[i]) / dir[i];
			} else {
				maxT[i] = -1.;
			}
		}

		// Get largest of the maxT's for final choice of intersection
		int whichPlane = 0;
		for ( int i = 1; i < 3; ++i ) {
			if ( maxT[whichPlane] < maxT[i] ) {
				whichPlane = i;
			}
		}

		// Check final candidate actually inside box
		if ( maxT[whichPlane] < 0 ) {
			return false;
		}

		double coord[3];
		for ( int i = 0; i < 3; ++i ) {
			if ( whichPlane != i ) {
				coord[i] = origin[i] + maxT[whichPlane] * dir[i];
				if ( coord[i] < minB[i] || coord[i] > maxB[i] ) {
					return false;
				}
			} else {
				coord[i] = candidatePlane[i];
			}
		}
		return true;
	}

	void BBox::reset ( )
	{
		clearChildren();
		this->depth = 0;
		face = NULL;
	}

	void BBox::clearChildren ( )
	{
		children.clear();
	}

	int BBox::getDepth ( ) const
	{
		return depth;
	}

	void BBox::setDepth ( const int depth )
	{
		this->depth = depth;
	}

	point3d BBox::getMin ( ) const
	{
		if ( minPoint.x == 0 && minPoint.y == 0 && minPoint.z == 0 ) {
			//Logger::error("bounding box minimum is (0,0,0)");
		}
		return minPoint;
	}

	point3d BBox::getMax ( ) const
	{
		if ( maxPoint.x == 0 && maxPoint.y == 0 && maxPoint.z == 0 ) {
			//Logger::error("bounding box maximum is (0,0,0)");
		}
		return maxPoint;
	}

	std::vector<BBox *> BBox::getChildren ( ) const
	{
		return children;
	}

	void BBox::addChild ( BBox * box )
	{
		children.push_back(box);

		for ( unsigned int i = 0; i < children.size() ; ++i ) {
			minPoint = minimumPoint(minPoint, children[i]->getMin());
			maxPoint = maximumPoint(maxPoint, children[i]->getMax());
		}
	}

	bool BBox::hasFace ( ) const
	{
		return face != NULL;
	}

	Face *BBox::getFace ( ) const
	{
		return face;
	}

	
	std::ostream &operator<< ( std::ostream &os, const BBox &box )
	{
		if ( box.hasFace() ) {
			os << " f";// << box.getMin();
		} else {
			os << std::endl;
			for ( int i = 0; i < box.getDepth(); ++i ) {
				os << "\t";
			}
			os << "|_" << " box " << &box << " (" << box.getDepth() << ")";
			std::vector<BBox *> children = box.getChildren();
			std::vector<BBox *>::const_iterator ci;
			for ( ci = children.begin(); ci != children.end(); ++ci ) {
				os << **ci;
			}
		}

		return os;
	}

}


namespace GFX
{

	BBoxEdge::BBoxEdge ( )
	{ }

	BBoxEdge::~BBoxEdge ( )
	{ }

	BBox *BBoxEdge::getBox ( ) const
	{
		return box;
	}

	double BBoxEdge::getValue ( ) const
	{
		return value;
	}

	bool BBoxEdge::isMin ( ) const
	{
		return start;
	}

	void BBoxEdge::set ( BBox *box, int dim, bool start )
	{
		this->box = box;
		this->start = start;

		if ( box == NULL ) return;

		switch ( dim ) {
			case 0:
				value = start ? box->getMin().x : box->getMax().x;
				break;
			case 1:
				value = start ? box->getMin().y : box->getMax().y;
				break;
			case 2:
				value = start ? box->getMin().z : box->getMax().z;
				break;
			default:
				value = 0;
		}
	}

	bool BBoxEdge::operator< ( const BBoxEdge &rhs ) const
	{
		return ( this->value < rhs.value || this->value == rhs.value && this->start && !rhs.start );
	}

	bool BBoxEdge::operator<= ( const BBoxEdge &rhs ) const
	{
		return ( this->value <= rhs.value || this->value == rhs.value && this->start );
	}

	bool BBoxEdge::operator> ( const BBoxEdge &rhs ) const
	{
		return ( this->value > rhs.value || this->value == rhs.value && !this->start );
	}
	
	bool BBoxEdge::operator>= ( const BBoxEdge &rhs ) const
	{
		return ( this->value >= rhs.value || this->value == rhs.value && !this->start );
	}

	bool BBoxEdge::operator== ( const BBoxEdge &rhs ) const
	{
		return ( this->value == rhs.value && this->start == rhs.start );
	}

	BBoxEdge &BBoxEdge::operator= ( const BBoxEdge &rhs )
	{
		this->value = rhs.value;
		this->start = rhs.start;
		this->box = rhs.box;
		return *this;
	}
}

