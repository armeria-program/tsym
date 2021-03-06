
#include "polyinfo.h"
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <cassert>
#include "basefct.h"
#include "logging.h"
#include "name.h"
#include "number.h"
#include "numberfct.h"
#include "order.h"
#include "poly.h"
#include "power.h"
#include "product.h"
#include "sum.h"
#include "undefined.h"

namespace tsym {
    namespace {
        /* Local Functor that determines the order or symbols to be used for multivariate gcd
         * computation. The object has state, i.e., the arguments u and v, for which the gcd shall
         * be computed. Thus, ordering will be different for different u/v, which is inevitable
         * here. The order is such that variables with least degree, but contained in both u and v
         * are first. Variables occuring only in one of u or v are placed at the end. */
        class ComparePolyVariables {
          public:
            ComparePolyVariables(const Base& u, const Base& v)
                : u(u)
                , v(v)
            {}

            bool operator()(const BasePtr& lhs, const BasePtr& rhs) const
            {
                const int lhsMinDegree = std::min(u.degree(*lhs), v.degree(*lhs));
                const int rhsMinDegree = std::min(u.degree(*rhs), v.degree(*rhs));

                if (lhsMinDegree != 0 && rhsMinDegree != 0)
                    return lhsMinDegree < rhsMinDegree;
                else if (lhsMinDegree == 0 && rhsMinDegree == 0)
                    return lhs->name() < rhs->name();
                else if (lhsMinDegree != 0)
                    return true;
                else
                    return false;
            }

          private:
            const Base& u;
            const Base& v;
        };

        bool isValidPower(const Base& power);
        bool hasValidOperands(const Base& arg);

        bool hasValidType(const Base& arg)
        /* Only symbols, rational Numerics, sums, products or powers with primitive int exponents are
         * allowed. */
        {
            if (isSymbol(arg))
                return true;
            else if (isNumeric(arg))
                return arg.numericEval()->isRational();
            else if (isPower(arg))
                return isValidPower(arg);
            else if (isSum(arg) || isProduct(arg))
                return hasValidOperands(arg);
            else
                return false;
        }

        bool isValidPower(const Base& power)
        {
            if (const auto exp = power.exp()->numericEval(); hasValidType(*power.base()) && exp)
                return isInt(*exp) && fitsInto<int>(exp->numerator()) && *exp > 0;

            return false;
        }

        bool hasValidOperands(const Base& arg)
        {
            using boost::adaptors::indirected;
            const auto valid = [](const auto& op) { return hasValidType(op); };

            return boost::algorithm::all_of(arg.operands() | indirected, valid);
        }
    }
}

bool tsym::poly::isInputValid(const Base& u, const Base& v)
{
    if (isZero(u) && isZero(v))
        return false;
    else
        return hasValidType(u) && hasValidType(v);
}

namespace tsym {
    namespace {
        void addIfNotAlreadyStored(BasePtrList& symbolList, const Base& symbol);
        void addSymbolsNonScalar(BasePtrList& symbolList, const Base& arg);
        void addSymbols(BasePtrList& symbolList, const BasePtrList& operands);

        void addSymbols(BasePtrList& symbolList, const Base& arg)
        {
            if (isSymbol(arg))
                addIfNotAlreadyStored(symbolList, arg);
            else if (isNumeric(arg))
                return;
            else
                addSymbolsNonScalar(symbolList, arg);
        }

        void addIfNotAlreadyStored(BasePtrList& symbolList, const Base& symbol)
        {
            const auto existing =
              boost::find_if(symbolList, [&symbol](const auto& other) { return symbol.isEqual(*other); });

            if (existing == cend(symbolList))
                symbolList.push_back(symbol.clone());
        }

        void addSymbolsNonScalar(BasePtrList& symbolList, const Base& arg)
        {
            if (isSum(arg))
                addSymbols(symbolList, arg.operands());
            else if (isProduct(arg))
                addSymbols(symbolList, arg.operands());
            else if (isPower(arg))
                addSymbols(symbolList, *arg.base());
        }

        void addSymbols(BasePtrList& symbolList, const BasePtrList& operands)
        {
            using boost::adaptors::indirected;

            for (const auto& op : operands | indirected)
                addSymbols(symbolList, op);
        }
    }
}

tsym::BasePtrList tsym::poly::listOfSymbols(const Base& u, const Base& v)
{
    BasePtrList symbolList;

    addSymbols(symbolList, u);
    addSymbols(symbolList, v);

    symbolList.sort(ComparePolyVariables(u, v));

    return symbolList;
}

namespace tsym {
    namespace {
        bool hasCommonSymbol(const BasePtrList& symbolList, const Base& u, const Base& v)
        {
            using boost::adaptors::indirected;

            return boost::algorithm::any_of(
              symbolList | indirected, [&u, &v](const auto& symbol) { return u.has(symbol) && v.has(symbol); });
        }
    }
}

tsym::BasePtr tsym::poly::mainSymbol(const BasePtrList& symbolList, const Base& u, const Base& v)
{
    if (hasCommonSymbol(symbolList, u, v))
        return symbolList.front();
    else
        return Undefined::create();
}
