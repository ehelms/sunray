#ifndef _REFLECTIVE_GROUND_TEST_H_
#define _REFLECTIVE_GROUND_TEST_H_

#include "Fit/ColumnFixture.h"

class ReflectiveGroundTest : public ColumnFixture
{
	public:
		ReflectiveGroundTest ( );
		virtual ~ReflectiveGroundTest ( ) { }
		
		/**
		 * @see SunRay.AcceptanceTests.ReflectiveComponent.PolarGridDefinition
		 */
		int phiForOrientation ( );
		int figureDirection;
		int orientation;
		
		/**
		 * @see SunRay.AcceptanceTests.ReflectiveComponent.PolarGridResolutionDegradation
		 */
		int numberOfZones ( );
		int phiResolution;
};

#endif // _REFLECTIVE_GROUND_TEST_H_
