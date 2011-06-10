#ifndef _SUNRAY_FIXTURE_MAKER_H_
#define _SUNRAY_FIXTURE_MAKER_H_

#include "Fit/FixtureMaker.h"
#include "Fit/FitnesseServer.h"

#include "reflectivegroundtest.h"

class SunrayFixtureMaker : public FixtureMaker
{
	public:
		explicit SunrayFixtureMaker ( ) { }
		virtual ~SunrayFixtureMaker ( ) { }

		virtual Fixture *make ( const std::string &name ) {
			PUBLISH_FIXTURE(ReflectiveGroundTest);
			return 0;
		}

	private:
		SunrayFixtureMaker ( const SunrayFixtureMaker & );
		SunrayFixtureMaker &operator= ( const SunrayFixtureMaker & );
};

#endif // _SUNRAY_FIXTURE_MAKER_H_
