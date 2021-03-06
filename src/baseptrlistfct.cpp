
#include "baseptrlistfct.h"
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/numeric.hpp>
#include "basefct.h"
#include "cache.h"
#include "logging.h"
#include "numeric.h"
#include "product.h"
#include "sum.h"

tsym::BasePtrList tsym::join(BasePtr&& first, BasePtrList&& second)
{
    second.push_front(std::move(first));

    return std::move(second);
}

tsym::BasePtrList tsym::join(BasePtrList&& first, BasePtrList&& second)
{
    first.splice(end(first), std::move(second));

    return std::move(first);
}

bool tsym::areEqual(const BasePtrList& list1, const BasePtrList& list2)
{
    return boost::equal(list1, list2, [](const auto& bp1, const auto bp2) { return bp1->isEqual(*bp2); });
}

tsym::BasePtrList tsym::rest(BasePtrList list)
{
    if (list.empty())
        TSYM_WARNING("Requesting rest of an empty list!");
    else
        list.pop_front();

    return list;
}

bool tsym::hasUndefinedElements(const BasePtrList& list)
{
    using boost::adaptors::indirected;

    return boost::algorithm::any_of(list | indirected, isUndefined);
}

bool tsym::hasZeroElements(const BasePtrList& list)
{
    using boost::adaptors::indirected;

    return boost::algorithm::any_of(list | indirected, isZero);
}

bool tsym::hasSumElements(const BasePtrList& list)
{
    using boost::adaptors::indirected;

    return boost::algorithm::any_of(list | indirected, isSum);
}

bool tsym::areAllElementsConst(const BasePtrList& list)
{
    return boost::algorithm::all_of(list, std::mem_fn(&Base::isConst));
}

unsigned tsym::complexitySum(const BasePtrList& list)
{
    return boost::accumulate(
      list, 0u, [](unsigned complexity, const auto& item) { return complexity + item->complexity(); });
}

tsym::BasePtrList tsym::getConstElements(const BasePtrList& list)
{
    BasePtrList items;

    boost::push_back(items, list | boost::adaptors::filtered(std::mem_fn(&Base::isConst)));

    return items;
}

tsym::BasePtrList tsym::getNonConstElements(const BasePtrList& list)
{
    using boost::adaptors::filtered;
    BasePtrList items;

    boost::push_back(items, list | filtered(std::not_fn(&Base::isConst)));

    return items;
}

namespace tsym {
    namespace {
        void defScalarAndSums(const BasePtrList& list, BasePtr& scalar, BasePtrList& sums)
        /* Splits the given product into a list of sums and everything else (termed 'scalar' here),
         * while the latter is saved as a product. */
        {
            BasePtrList scalarFactors;

            for (const auto& item : list) {
                if (const auto expanded = item->expand(); isSum(*expanded))
                    sums.push_back(expanded);
                else
                    scalarFactors.push_back(expanded);
            }

            scalar = scalarFactors.empty() ? Numeric::one() : Product::create(scalarFactors);
        }

        BasePtr expandProductOf(BasePtrList& sums)
        /* Recursively expands a the sum terms of a product, e.g. (a + b)*(c + d) = a*c + a*d + b*c +
         * b*d. */
        {
            const BasePtr first(sums.front());
            BasePtrList summands;
            BasePtr second;

            sums.pop_front();

            if (sums.empty())
                return first;

            second = sums.front();
            sums.pop_front();

            for (const auto& item : first->operands())
                summands.push_back(Product::create(item, second)->expand());

            sums.push_front(Sum::create(summands));

            return expandProductOf(sums);
        }

        BasePtr expandProductOf(const BasePtr& scalar, const Base& sum)
        {
            BasePtrList summands;

            assert(isSum(sum));

            for (const auto& item : sum.operands()) {
                auto product = Product::create(scalar, item);
                summands.push_back(product->expand());
            }

            return Sum::create(std::move(summands));
        }
    }
}

tsym::BasePtr tsym::expandAsProduct(const BasePtrList& list)
{
    static RegisteredCache<BasePtrList, BasePtr> cache;
    static auto& map(cache.map);
    const auto lookup = map.find(list);
    BasePtr expanded;
    BasePtrList sums;
    BasePtr scalar;

    if (lookup != cend(map))
        return lookup->second;

    defScalarAndSums(list, scalar, sums);

    if (sums.empty())
        expanded = scalar;
    else {
        const BasePtr secondFactor = expandProductOf(sums);

        if (isSum(*secondFactor))
            expanded = expandProductOf(scalar, *secondFactor);
        else
            expanded = Product::create(scalar, secondFactor);
    }

    return map.insert({list, std::move(expanded)})->second;
}

void tsym::subst(BasePtrList& list, const Base& from, const BasePtr& to)
{
    for (auto& item : list)
        item = item->subst(from, to);
}

tsym::BasePtrList tsym::subst(const BasePtrList& list, const Base& from, const BasePtr& to)
{
    BasePtrList res;

    for (const auto& item : list)
        res.push_back(item->subst(from, to));

    return res;
}
