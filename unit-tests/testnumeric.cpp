
#include "basefct.h"
#include "fixtures.h"
#include "name.h"
#include "numeric.h"
#include "tsymtests.h"

using namespace tsym;

BOOST_FIXTURE_TEST_SUITE(TestNumeric, AbcFixture)

BOOST_AUTO_TEST_CASE(creationByNumber)
{
    const Number half(1, 2);
    const BasePtr ptr = Numeric::create(half);

    BOOST_TEST(isNumeric(*ptr));
    BOOST_CHECK_EQUAL(half, ptr->numericEval().value());
}

BOOST_AUTO_TEST_CASE(creationByIntegerClass)
{
    const Int num = 400;
    const Int denom = 200;
    const BasePtr res = Numeric::create(num, denom);

    BOOST_CHECK_EQUAL(two, res);
}

BOOST_AUTO_TEST_CASE(creationWithZeroDenominator, noLogs())
{
    const BasePtr ptr = Numeric::create(1, 0);

    BOOST_TEST(isUndefined(*ptr));
}

BOOST_AUTO_TEST_CASE(creationByInt)
{
    const BasePtr ptr = Numeric::one();

    BOOST_TEST(isNumeric(*ptr));
    BOOST_CHECK_EQUAL(1, ptr->numericEval().value());
}

BOOST_AUTO_TEST_CASE(creationByFraction)
{
    const BasePtr res = Numeric::fourth();

    BOOST_TEST(isNumeric(*res));
    BOOST_CHECK_EQUAL(Number(1, 4), res->numericEval().value());
}

BOOST_AUTO_TEST_CASE(creationByDouble)
{
    const double value = 1.23456789;
    const BasePtr res = Numeric::create(value);

    BOOST_TEST(isNumeric(*res));
    BOOST_CHECK_EQUAL(value, res->numericEval().value());
}

BOOST_AUTO_TEST_CASE(numericEvaluation)
{
    const Number expected(2, 3);
    const BasePtr frac = Numeric::create(2, 3);

    BOOST_CHECK_EQUAL(expected, frac->numericEval().value());
}

BOOST_AUTO_TEST_CASE(typeString)
{
    const std::string expected("Numeric");
    const BasePtr ptr = Numeric::create(1.23456);

    BOOST_CHECK_EQUAL(expected, ptr->typeStr());
}

BOOST_AUTO_TEST_CASE(uselessNameRequest)
{
    const BasePtr n = Numeric::create(2, 3);
    const Name empty;

    BOOST_CHECK_EQUAL(empty, n->name());
}

BOOST_AUTO_TEST_SUITE_END()
