/**
*   @author Joonatan Kuosa <joonatan.kuosa@gmail.com>
*   @date 2018-05
*   @file main.cpp
*
*   Under a copyleft.
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include "prime.hpp"
#include "chrono.hpp"
#include "defines.hpp"

int main(int argc, char **argv)
{
    // Redirect cout
    // simpler to print into it, but console is slow as sin
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ofstream fout("output_single_t.txt");
    std::cout.rdbuf(fout.rdbuf());

    std::stringstream ss;
    ss << "Startin non-threaded version: with" << std::endl
        << " " << N_PRIMES << " numbers to check for primes." << std::endl
        << " With a delay of " << DELAY << "ms per function call.";
    // Print to the user
    std::clog << ss.str() << std::endl;
    // print to log
    std::cout << ss.str() << std::endl;
    vl::chrono app_clock;
    size_t count = 0;
    for(size_t i = 0; i < N_THREADS*BATCH_SIZE*N_RUNS; ++i)
    {
        really_slow_func(DELAY);
        if(isPrime(i))
        {
            std::cout << i << " is a prime " << std::endl;
            ++count;
        }
    }

    ss.str("");
    ss << "ALL DONE " << std::endl
        << "Found " << count << " primes." << std::endl
        << "Total time: " << app_clock.elapsed();
    std::cout << ss.str() << std::endl;
    std::clog << ss.str() << std::endl;

    return 0;
}
