#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include <iostream>
#include <cmath>

namespace GFX
{
	const double ZERO_TOLERANCE = 1e-6;

	typedef struct {
		double x;
		double y;
		double z;
	} point3d, vector3d;

	typedef struct {
		double r;
		double theta;
		double phi;
	} polar3d;

	polar3d toPolar ( const vector3d );
	vector3d toCartesian ( const polar3d );
	double magnitude ( const vector3d );
	polar3d rotatePolar ( const polar3d, const polar3d, const double );
	vector3d rotateCartesian ( const vector3d, const vector3d, const double );
	double dot ( const vector3d, const vector3d );

	bool operator==(const point3d &p1,const point3d &p2);
	bool operator!=(const point3d &p1,const point3d &p2);

	point3d operator+(const point3d &p1,const point3d &p2);
	point3d operator-(const point3d &p1,const point3d &p2);
	point3d operator-(const point3d &p1);
	point3d operator*(const point3d &p1,const point3d &p2);
	point3d operator/(const point3d &p1,const point3d &p2);
	point3d operator*(const float s,const point3d &p1);

	point3d operator*(const point3d &p1,const float s);
	point3d operator/(const point3d &p1,const float s);
	point3d operator+(const point3d &p1,const float s);
	point3d operator-(const point3d &p1,const float s);
	void operator+=(point3d &p1,const point3d &p2);
	void operator-=(point3d &p1,const point3d &p2);
	void operator/=(point3d &p1,const point3d &p2);
	void operator*=(point3d &p1,const point3d &p2);
	void operator/=(point3d &p1,const float s);
	void operator*=(point3d &p1,const float s);
	void operator+=(point3d &p1,const float s);
	void operator-=(point3d &p1,const float s);
	std::ostream& operator<<(std::ostream& os,const point3d &p);
	std::ostream& operator<<(std::ostream& os,const polar3d &p);
	bool operator<(const point3d &p1,const point3d &p2);
	bool operator>(const point3d &p1,const point3d &p2);
	bool operator<=(const point3d &p1,const point3d &p2);
	bool operator>=(const point3d &p1,const point3d &p2);
	inline double mag(const point3d &p1){
		return std::sqrt(std::abs(p1.x*p1.x+p1.y*p1.y+p1.z*p1.z));
	}
	inline point3d normalize(const point3d &p1){
		double m=mag(p1);
		if(m>0)return p1/m;
		return p1;
	}
	inline point3d cross(const point3d &p1,const point3d &p2){
		point3d p3={p1.y*p2.z-p1.z*p2.y,p1.z*p2.x-p1.x*p2.z,p1.x*p2.y-p1.y*p2.x};
		return p3;
	}
	inline point3d minimumPoint(const point3d &p1,const point3d &p2){
		point3d p3={std::min(p1.x,p2.x),std::min(p1.y,p2.y),std::min(p1.z,p2.z)};
		return p3;
	}
	inline point3d maximumPoint(const point3d &p1,const point3d &p2){
		point3d p3={std::max(p1.x,p2.x),std::max(p1.y,p2.y),std::max(p1.z,p2.z)};
		return p3;
	}

	inline point3d minimumPoint(const point3d &p1,const point3d &p2,const point3d &p3){
		return minimumPoint(minimumPoint(p1, p2), p3);
	}
	inline point3d maximumPoint(const point3d &p1,const point3d &p2,const point3d &p3){
		return maximumPoint(maximumPoint(p1, p2), p3);
	}
	double* quadratic(double a,double b,double c);
	const point3d ORIGIN={0,0,0};
}
#endif // _PRIMITIVES_H_
