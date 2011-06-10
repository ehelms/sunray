/**
 * driver for test CppUnit test suite
 */
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "simulation/solarsimulationtest.h"
#include "simulation/sunrayradiativetransfermodeltest.h"

int main ( int argc, char *argv[] )
{
	CPPUNIT_NS::TextTestRunner runner;
	CPPUNIT_NS::CompilerOutputter outputter(&runner.result(), std::cerr);
	runner.setOutputter(&outputter);
	runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	runner.run("", true);

	return 0;
}

