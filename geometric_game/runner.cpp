/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "testsuite.h"

static TestEuclidean suite_TestEuclidean;

static CxxTest::List Tests_TestEuclidean = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestEuclidean( "testsuite.h", 4, "TestEuclidean", suite_TestEuclidean, Tests_TestEuclidean );

static class TestDescription_TestEuclidean_testDifference : public CxxTest::RealTestDescription {
public:
 TestDescription_TestEuclidean_testDifference() : CxxTest::RealTestDescription( Tests_TestEuclidean, suiteDescription_TestEuclidean, 7, "testDifference" ) {}
 void runTest() { suite_TestEuclidean.testDifference(); }
} testDescription_TestEuclidean_testDifference;

#include <cxxtest/Root.cpp>
