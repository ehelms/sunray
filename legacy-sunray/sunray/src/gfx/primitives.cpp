#include "primitives.h"

#include <cmath>

#include "../logger.h"

namespace GFX
{

	polar3d toPolar ( const vector3d cart )
	{
		double r = magnitude(cart);
		polar3d retr = { r, std::atan2(cart.y, cart.x), (r == 0) ? 0 : std::acos(cart.z / r) };
		return retr;
	}

	vector3d toCartesian ( const polar3d pol )
	{
		vector3d retr = { pol.r * std::cos(pol.theta) * std::sin(pol.phi), pol.r * std::sin(pol.theta) * std::sin(pol.phi), pol.r * std::cos(pol.phi) };
		return retr;
	}
	
	double magnitude ( const vector3d v )
	{
		return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	}

	polar3d rotatePolar ( const polar3d initial, const polar3d axis, const double angle )
	{
		polar3d retr = { 0.0, 0.0, 0.0 };
		return retr;
	}

	vector3d rotateCartesian ( const vector3d initial, const vector3d axis, const double angle )
	{
		double x = initial.x, y = initial.y, z = initial.z;
		double u = axis.x, v = axis.y, w = axis.z;
		double d = dot(initial, axis);
		double mag = magnitude(axis);
		double invMag2 = 1.0 / (mag*mag);
		double cosine = std::cos(angle);
		double sine = std::sin(angle);

		vector3d result;
		result.x = (u*d + (x*(v*v + w*w) - u*(v*y + w*z))*cosine + mag*(-w*y + v*z)*sine) * invMag2;
		result.y = (v*d + (y*(u*u + w*w) - v*(u*x + w*z))*cosine + mag*(w*x - u*z)*sine) * invMag2;
		result.z = (w*d + (z*(u*u + v*v) - w*(u*x + v*y))*cosine + mag*(-v*x + u*y)*sine) * invMag2;
		return result;
	}

	double dot ( const vector3d lhs, const vector3d rhs )
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}
	bool operator== ( const point3d &p1, const point3d &p2 )
	{
		return (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z);
	}

	bool operator!= ( const point3d &p1, const point3d &p2 )
	{
		return !(p1 == p2);
	}

	point3d operator+ ( const point3d &p1, const point3d &p2 )
	{
		point3d p3 = { p1.x + p2.x, p1.y + p2.y, p1.z + p2.z };
		return p3;
	}

	point3d operator- ( const point3d &p1, const point3d &p2 )
	{
		point3d p3 = { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
		return p3;
	}

	point3d operator- ( const point3d &p1 )
	{
		point3d p3 = { -p1.x, -p1.y, -p1.z };
		return p3;
	}

	point3d operator* ( const point3d &p1, const point3d &p2 )
	{
		point3d p3 = { p1.x * p2.x, p1.y * p2.y, p1.z * p2.z };
		return p3;
	}
	
	point3d operator/ ( const point3d &p1, const point3d &p2 )
	{
		point3d p3 = { p1.x / p2.x, p1.y / p2.y, p1.z / p2.z };
		return p3;
	}

	point3d operator* ( const float s, const point3d &p1 )
	{
		point3d p3 = { p1.x * s, p1.y * s, p1.z * s };
		return p3;
	}

	point3d operator* ( const point3d &p1, const float s )
	{
		point3d p3 = { p1.x * s, p1.y * s, p1.z * s };
		return p3;
	}

	point3d operator/ ( const point3d &p1, const float s )
	{
		point3d p3 = { p1.x / s, p1.y / s, p1.z / s };
		return p3;
	}

	point3d operator+ ( const point3d &p1, const float s )
	{
		point3d p3 = { p1.x + s, p1.y + s, p1.z + s };
		return p3;
	}

	point3d operator- ( const point3d &p1, const float s )
	{
		point3d p3 = { p1.x - s, p1.y - s, p1.z - s };
		return p3;
	}

	void operator+= ( point3d &p1, const point3d &p2 )
	{
		p1.x += p2.x;
		p1.y += p2.y;
		p1.z += p2.z;
	}

	void operator-= ( point3d &p1, const point3d &p2 )
	{
		p1.x -= p2.x;
		p1.y -= p2.y;
		p1.z -= p2.z;
	}

	void operator/= ( point3d &p1, const point3d &p2 )
	{
		p1.x /= p2.x;
		p1.y /= p2.y;
		p1.z /= p2.z;
	}
	
	void operator*= ( point3d &p1, const point3d &p2 )
	{
		p1.x *= p2.x;
		p1.y *= p2.y;
		p1.z *= p2.z;
	}

	void operator/= ( point3d &p1, const float s )
	{
		p1.x /= s;
		p1.y /= s;
		p1.z /= s;
	}

	void operator*= ( point3d &p1, const float s )
	{
		p1.x *= s;
		p1.y *= s;
		p1.z *= s;
	}

	void operator+= ( point3d &p1, const float s )
	{
		p1.x += s;
		p1.y += s;
		p1.z += s;
	}

	void operator-= ( point3d &p1, const float s )
	{
		p1.x -= s;
		p1.y -= s;
		p1.z -= s;
	}

	std::ostream &operator<< ( std::ostream &os, const point3d &p )
	{
		return os << "[" << p.x << ", " << p.y << ", " << p.z << "]";
	}

	std::ostream &operator<< ( std::ostream &os, const polar3d &p )
	{
		return os << "[" << p.r << ", " << p.theta << ", " << p.phi << "]";
	}

	bool operator< ( const point3d &p1, const point3d &p2 )
	{
		return (p1.x < p2.x && p1.y < p2.y && p1.z < p2.z);
	}

	bool operator> ( const point3d &p1, const point3d &p2 )
	{
		return (p1.x > p2.x && p1.y > p2.y && p1.z > p2.z);
	}

	bool operator<= ( const point3d &p1, const point3d &p2 )
	{
		return (p1.x <= p2.x && p1.y <= p2.y && p1.z <= p2.z);
	}

	bool operator>= ( const point3d &p1, const point3d &p2 )
	{
		return (p1.x >= p2.x && p1.y >= p2.y && p1.z >= p2.z);
	}
}

