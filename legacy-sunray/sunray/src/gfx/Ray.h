#ifndef _RAY_H_
#define _RAY_H_

#include "primitives.h"

namespace GFX
{

	class Ray
	{

		private:
			point3d p;
			vector3d v;
			double t;
		public:
			Ray(){
				p=ORIGIN;
				v=ORIGIN;
				t=0;
			}
			Ray(const point3d &p,const vector3d &v){
				this->p=p;
				this->v=v;
				this->v/=mag(this->v);
				t=0;
			}
			void set(const point3d &p,const vector3d &v){
				this->p=p;
				this->v=v;
				this->v/=mag(this->v);
				t=0;
			}
			double getTime() const {return t;}
			inline void setTime(double t){this->t=t;}
			inline point3d getOrigin() const {return p;}
			inline vector3d getDirection() const {return v;}
	};
}
#endif // _RAY_H_
