#ifndef TSYM_POWERSIMPL_H
#define TSYM_POWERSIMPL_H

#include "baseptr.h"
#include "baseptrlist.h"

namespace tsym { class Number; }

namespace tsym {
    class PowerSimpl {
        /* Processes the simplification of power expressions. A BasePtrList with 2 items is
         * returned, base first, exponent second. If the latter may be one, which makes it possible
         * to return products (e.g. sqrt(8) = 2*sqrt(2)) or numerics (e.g. sqrt(4) = 2). For details
         * of the simplification of purely numeric powers, see the NumPowerSimpl class.
         *
         * One of the major differences in comparison to convential CAS is implemented here: as
         * symbols are considered real numbers, some simplifications work out, that can't be done
         * for possibly complex numbers, e.g. (a^3)^pi = a^(3*pi) or (a^7)^(1/4) = a^(7/4) because
         * no information is lost during this step (if a < 0, both terms are undefined, and they are
         * equivalent for a > 0, but this isn't true for a complex base a). The inverse happens as
         * well: (a^(1/3))^3 is simplified to a in most CAS (based on a = i^2*|a| for a < 0), but
         * not here, because a^(1/3) is simply undefined for a < 0. */
        public:
            PowerSimpl();

            BasePtrList simplify(const BasePtr& base, const BasePtr& exp);

        private:
            bool doesInvolveComplexNumbers(const BasePtr& base, const BasePtr& exp);
            BasePtrList simplifyNumericBase(const BasePtr& base, const BasePtr& exp);
            BasePtrList simplifyNumericPower(const BasePtr& base, const BasePtr& exp);
            BasePtrList simplifyNumericPower(const Number& base, const Number& exp);
            BasePtrList simplifyPowerBase(const BasePtr& powBase, const BasePtr& e2);
            bool doContractExpFirst(const BasePtr& base, const BasePtr& e1, const BasePtr& e2);
            bool areTwoIntegerExp(const BasePtr& exp1, const BasePtr& exp2);
            bool isInteger(const BasePtr& arg);
            bool areTwoFractionExpWithOddDenom(const BasePtr& exp1, const BasePtr& exp2);
            bool doesChangeSign(const BasePtr& base, const BasePtr& exp1);
            bool doContractExpSecond(const BasePtr& e1, const BasePtr& e2);
            bool isOddInteger(const BasePtr& arg);
            bool isEvenInteger(const BasePtr& arg);
            bool isFraction(const BasePtr& arg);
            BasePtrList simplifyProductBase(const BasePtr& base, const BasePtr& exp);
            BasePtrList simplifyConstantBase(const BasePtr& base, const BasePtr& exp);
            bool isBaseEulerConstantAndExpLogarithm(const BasePtr& base, const BasePtr& exp);

            const BasePtr one;
    };
}

#endif
