#include "bsphere.h"

namespace GFX
{

	BSphere::BSphere ( ) :
		radius(0)
	{
		center.x = center.y = center.z = 0;
	}

	BSphere::BSphere ( const double r, const point3d &p ) :
		radius(r),
		center(p)
	{ }

	BSphere::~BSphere ( )
	{ }

	void BSphere::setRadius ( const double r )
	{
		radius = r;
	}

	void BSphere::setCenter ( const point3d &p )
	{
		center.x = p.x;
		center.y = p.y;
		center.z = p.z;
	}

	double BSphere::getRadius ( ) const
	{
		return radius;
	}

	point3d BSphere::getCenter ( ) const
	{
		return center;
	}

	void BSphere::addVertex ( const point3d &p )
	{
		radius = std::max(radius, GFX::magnitude(p - center));
	}

	void BSphere::reset ( )
	{
		radius = 0;
		center.x = center.y = center.z = 0;
	}

	point3d BSphere::getPoint ( const double theta, const double phi )
	{
		point3d p = { radius * cos(theta) * sin(phi), radius * sin(theta) * sin(phi), radius * cos(phi) };
		p += center;
		return p;
	}

}

