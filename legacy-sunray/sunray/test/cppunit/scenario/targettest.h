/**
 * Automated unit tests for Target
 *
 * @copyright 2007 Computational Photobiology Research Group
 * @author Josiah Gore
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * A copy of the GNU General Public License was provided along with 
 * this program, and is also available at <http://www.gnu.org/licenses/>.
 */

#ifndef _TARGET_TEST_H_
#define _TARGET_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

// put this at the top of the TargetTest.cpp file to add this 
// specific test suite to the full suite of tests
//CPPUNIT_TEST_SUITE_REGISTRATION(TargetTest);

class TargetTest : public CPPUNIT_NS::TestFixture
{
	// cppunit macros to create a suite of tests
	CPPUNIT_TEST_SUITE(TargetTest);
	CPPUNIT_TEST(testPassingTestCase);
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

	private:
		// helper variables

};

#endif // _TARGET_TEST_H_


