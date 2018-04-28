
#define BOOST_TEST_MODULE tsym unit tests
#include <boost/test/unit_test.hpp>
#include "cache.h"
#include "fixtures.h"

struct GlobalFixture : public tsym::TestSuiteLoggingFixture {
    GlobalFixture()
        : tsym::TestSuiteLoggingFixture(false)
    {}

    ~GlobalFixture()
    {
        tsym::cache::clearRegisteredCaches();
    }
};

BOOST_TEST_GLOBAL_FIXTURE(GlobalFixture);
