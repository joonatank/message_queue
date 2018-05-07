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

/// Params {EXE} {N_THREADS} {DELAY} {OUTPUT_FILENAME}
/// threads doesn't actually create threads but affects how many primes we calculate
/// Delay in milliseconds (extra time function call takes)
int main(int argc, char **argv)
{
    // Parse input args
    int n_threads = N_THREADS;
    int delay = DELAY;
    std::string out_filename = "output_single_t.txt";

    if(argc > 1)
    {
        n_threads = std::atoi(argv[1]);
    }
    if(argc > 2)
    {
        delay = std::atoi(argv[2]);
    }
    if(argc > 3)
    {
        out_filename = argv[3];
    }

    // Redirect cout
    // simpler to print into it, but console is slow as sin
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ofstream fout(out_filename);
    std::cout.rdbuf(fout.rdbuf());

    /// Total number of primes to calculate
    const size_t N_PRIMES = n_threads * BATCH_SIZE * N_RUNS;

    std::stringstream ss;
    ss << "Startin non-threaded version: with" << std::endl
        << " " << N_PRIMES << " numbers to check for primes." << std::endl
        << " With a delay of " << delay << "ms per function call.";
    // Print to the user
    std::clog << ss.str() << std::endl;
    // print to log
    std::cout << ss.str() << std::endl;
    vl::chrono app_clock;
    size_t count = 0;
    for(size_t i = 0; i < n_threads*BATCH_SIZE*N_RUNS; ++i)
    {
        really_slow_func(delay);
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
