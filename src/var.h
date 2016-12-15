#ifndef TSYM_VAR_H
#define TSYM_VAR_H

#include <vector>
#include <utility>
#include <string>

namespace tsym { class BasePtr; }

namespace tsym {
    class Var {
        public:
            enum Sign { POSITIVE = 1 };

            Var();
            Var(int value);
            Var(double value);
            Var(int numerator, int denominator);
            /* The next two constructors create symbols. Alphanumeric characters are valid, though
             * the symbol name must not start with a digit. It can contain subscripts though: if
             * it's only one character, a_1 is sufficient, for longer subscripts, use a_{10}. */
            explicit Var(const char *name);
            explicit Var(const char *name, Sign sign);
            /* To be used only internally: */
            explicit Var(const BasePtr& ptr);
            Var(const Var& other);
            const Var& operator = (const Var& rhs);
            ~Var();

            Var& operator += (const Var& rhs);
            Var& operator -= (const Var& rhs);
            Var& operator *= (const Var& rhs);
            Var& operator /= (const Var& rhs);

            const Var& operator + () const;
            Var operator - () const;

            Var toThe(const Var& exponent) const;

            Var subst(const Var& from, const Var& to) const;
            Var expand() const;
            Var normal() const;
            /* The argument must be a Symbol: */
            Var diff(const Var& symbol) const;
            bool equal(const Var& other) const;
            bool has(const Var& other) const;
            bool isZero() const;
            bool isPositive() const;
            bool isNegative() const;
            /* Returns "Symbol", "Integer", "Fraction", "Double", "Constant", "Undefined",
             * "Function", "Sum", "Product" or "Power": */
            std::string type() const;
            Var numerator() const;
            Var denominator() const;
            bool fitsIntoInt() const;
            int toInt() const;
            double toDouble() const;
            /* Returns names for Symbols, Functions and Constants, an empty string otherwise: */
            const std::string& name() const;
            std::vector<Var> operands() const;
            std::vector<Var> collectSymbols() const;
            const BasePtr& getBasePtr() const;

        private:
            std::string numericType() const;
            bool isInteger() const;
            std::pair<Var, Var> normalToFraction() const;
            void collectSymbols(const BasePtr& ptr, std::vector<Var>& symbols) const;
            void insertSymbolIfNotPresent(const BasePtr& symbol, std::vector<Var>& symbols) const;

            BasePtr *rep;
    };

    bool operator == (const Var& lhs, const Var& rhs);
    bool operator != (const Var& lhs, const Var& rhs);

    Var operator + (Var lhs, const Var& rhs);
    Var operator - (Var lhs, const Var& rhs);
    Var operator * (Var lhs, const Var& rhs);
    Var operator / (Var lhs, const Var& rhs);

    std::ostream& operator << (std::ostream& stream, const Var& rhs);
}

#endif
