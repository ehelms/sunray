/**
 * Automated unit testing fixture example
 */

#ifndef _EXAMPLE_TEST_H_
#define _EXAMPLE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

// put this at the top of the *Test.cpp file to add this 
// specific test suite to the full suite of tests
//CPPUNIT_TEST_SUITE_REGISTRATION(ExampleTest);

class ExampleTest : public CPPUNIT_NS::TestFixture
{
	// cppunit macros to create a suite of tests
	CPPUNIT_TEST_SUITE(ExampleTest);
	CPPUNIT_TEST(testPassingTestCase);
	CPPUNIT_TEST(testFailingTestCase);
	CPPUNIT_TEST_SUITE_END();

	public:
		// use these functions to prepare for and clean up after each test
		void setUp ( ) { }
		void tearDown ( ) { }

	protected:
		// exercise your code here
		void testPassingTestCase ( )
		{
			CPPUNIT_ASSERT(true);
		}

		void testFailingTestCase ( )
		{
			CPPUNIT_ASSERT(false);
		}

	private:
		// helper variables

};

#endif // _EXAMPLE_TEST_H_

