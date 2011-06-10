#ifndef _SCENARIO_CREATOR_H_
#define _SCENARIO_CREATOR_H_

#include "scenario.h"

namespace Simulation
{

	class ScenarioCreator
	{
		public:
			virtual Scenario *createScenario ( ) = 0;

			virtual ~ScenarioCreator ( ) { }
	};
}

#endif // _SCENARIO_CREATOR_H_

