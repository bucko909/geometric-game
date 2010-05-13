#include <cxxtest/TestSuite.h>
#include "euc.hpp"

using namespace bucko;

class TestEuclidean : public CxxTest::TestSuite
{
public:
	void testDifference ( void )
	{
		EucPoint3<float> x(1, 2, 3);
		EucPoint3<float> y(3, 5, 7);
		EucVector3<float> z(-2, -3, -4);
		EucVector3<float> w(x - y);
		TS_ASSERT( w.x == z.x );
		TS_ASSERT( w.y == z.y );
		TS_ASSERT( w.z == z.z );
	}
};
