#include "factorization.hpp"

#include <map>
#include <cassert>
                                                

// Trial division algorithm of integer factorization.
// https://en.wikipedia.org/wiki/Trial_division
// Input - integer > 0, output map of prime factors and power.
void factorization(Number num, std::map<uint64_t, uint64_t> &prime_factors) {
    assert(num > 0);

    if (num < 2) {
        return;
    }

    Number p = 2; //factor

    while (p*p <= num) {
        while (num % p == 0) {
            prime_factors[p] += 1;
            num /= p;
        }
        ++p;
    }

    if (num > 1) {
        prime_factors[num] += 1; 
    }

    return;
}

