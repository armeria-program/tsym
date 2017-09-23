
#include <memory>
#include "globals.h"
#include "printer.h"
#include "numeric.h"
#include "logarithm.h"
#include "trigonometric.h"
#include "baseptrlist.h"
#include "constant.h"
#include "matrix.h"
#include "poly.h"
#include "name.h"
#include "abc.h"
#include "logging.h"
#include "testsuitelogger.h"
#include "tsymtests.h"
#include "CppUTest/CommandLineTestRunner.h"

void initConstructOnFirstUse()
    /* This initialization ensures that CppUTest doesn't spot a memory leak for the first test cases
     * that call the following functions employing local static variables. */
{
    const tsym::BasePtrList emptyList;
    const tsym::Matrix emptyMatrix;
    const tsym::Vector emptyVec;
    tsym::BasePtr undefined;
    tsym::Number n(1);

    n /= n;
    n -= n;

    tsym::Name("dummy").tex();

    tsym::Numeric::zero();
    tsym::Numeric::one();
    tsym::Numeric::mOne();

    tsym::Logarithm::create(tsym::Constant::createE());

    tsym::Trigonometric::createAtan2(zero, two);

    tsym::poly::gcd(tsym::Numeric::one(), tsym::Numeric::one());

    emptyList.front();
    emptyList.back();

    emptyMatrix(1, 1);
    emptyVec(1);

    tsym::Var("a").type();
}

int main(int argc, char** argv)
{
    auto logger = std::make_shared<const TestSuiteLogger>("misc/test-logfiles/debug.log",
            "misc/test-logfiles/info.log");

    tsym::Logger::setInstance(logger);

    tsym::setOption(tsym::Option::USE_OPTIONAL_CACHING, false);

    disableLog();
    initConstructOnFirstUse();
    enableLog();

    tsym::Printer::disableFractions();

    return CommandLineTestRunner::RunAllTests(argc, argv);
}
