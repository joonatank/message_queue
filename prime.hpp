/**
*   @author Joonatan Kuosa <joonatan.kuosa@gmail.com>
*   @date 2018-05
*   @file fifo.hpp
*
*   Under a copyleft.
*/

#ifndef PRIME_HPP
#define PRIME_HPP

#include "chrono.hpp"

/// @brief test if a number is a prime or not
/// @param n number to test
/// @return true if prime, false otherwise
bool isPrime(size_t n)
{
    if (n <= 1)
    { return false; }
    else if (n <= 3)
    { return true; }
    else if (n % 2 == 0 || n % 3 == 0)
    { return false; }

    int i = 5;
    while (i * i <= n)
    {
        if (n % i == 0 || n %(i + 2) == 0)
        { return false; }
        i = i + 6;
    }
    return true;
}

/// @brief delay function that simulates a more complex function call
/// @param delay_ms empty spinning time in milliseconds
void really_slow_func(double delay_ms)
{
    auto limit = vl::time(0, (uint32_t)(delay_ms*1000));
    auto clock = vl::chrono();

    while(clock.elapsed() < limit)
    {}
}

#endif  // PRIME_HPP
