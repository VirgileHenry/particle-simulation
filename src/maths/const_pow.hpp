// from https://stackoverflow.com/questions/16443682/c-power-of-integer-template-meta-programming
// allow to do compile time exponent, for template usage.


template<class T>
inline constexpr T const_pow(const T base, unsigned const exponent)
{
    // (parentheses not required in next line)
    return (exponent == 0) ? 1 : (base * pow(base, exponent-1));
}