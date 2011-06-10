/**
 * Automated unit tests for Actor
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

#ifndef _ACTOR_TEST_H_
#define _ACTOR_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "scenario/actor.h"
#include "scenario/solarray.h"

class IntersectingRay : private SolarRay

class MockActor : private Actor
{
	public:
		MockActor ( ) { }
		~MockActor ( ) { }

		bool intersects ( const SolarRay &source, SolarRay &res ) {
			if ( typeid(source) == typeid(IntersectingRay) ) {
				return true;
			} else {
				return false;
			}
		}
}

class ActorTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(ActorTest);
	CPPUNIT_TEST();
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp ( ) 
		{
			actor = new MockActor;
		}

		void tearDown ( ) 
		{
			delete actor;
		}

	protected:
		void testIntersects ( )
		{
			IntersectingRay passRay;
			Ray failRay;

			CPPUNIT_ASSERT(actor->intersects(passRay));
			CPPUNIT_ASSERT(! actor->intersects(failRay));
		}

	private:
		Actor *actor;

};

CPPUNIT_TEST_SUITE_REGISTRATION(ActorTest);

#endif // _ACTOR_TEST_H_

