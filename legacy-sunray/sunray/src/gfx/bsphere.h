#ifndef _BSPHERE_H_
#define _BSPHERE_H_

#include "primitives.h"

namespace GFX
{

	class BSphere
	{
		public:
			BSphere ( );
			BSphere ( double, const point3d & );
			virtual ~BSphere();

			void setRadius ( const double );
			void setCenter ( const point3d & );

			double getRadius ( ) const;
			point3d getCenter ( ) const;

			void addVertex ( const point3d & );
			void reset ( );

			point3d getPoint(double theta,double phi);

		protected:
			double radius;
			point3d center;

	};
}
#endif // _BSPHERE_H_
