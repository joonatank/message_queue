/**
*	@author Joonatan Kuosa <joonatan.kuosa@gmail.com>
*	@date 2018-05
*	@file fifo.hpp
*
*	Under a copyleft.
*/

#ifndef PRIME_HPP
#define PRIME_HPP

/// @brief test if a number is a prime or not
/// @param n number to test
/// @return true if prime, false otherwise
bool isPrime(int n)
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

#endif  // PRIME_HPP