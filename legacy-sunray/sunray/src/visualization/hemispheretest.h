#ifndef _HEMISPHERE_TEST_H_
#define _HEMISPHERE_TEST_H_

#include "visualization.h"

#include "../gfx/primitives.h"

namespace Visualization
{
	const double PI = 3.141592654;

	class HemisphereTest : public Visualization
	{

		public:
			HemisphereTest ( );
			virtual ~HemisphereTest ( );

			void run ( );

			void setNormal ( const double, const double, const double );
			void setThetaResolution ( const double theta );
			void setPhiResolution ( const double phi );

		protected:
			GFX::vector3d normal;
			double thetaRes;
			double phiRes;

	};

}

#endif // _HEMISPHERE_TEST_H_

