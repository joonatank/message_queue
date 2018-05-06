/**
*   @author Joonatan Kuosa <joonatan.kuosa@gmail.com>
*   @date 2018-05
*   @file defines.hpp
*
*   Under a copyleft.
*/

#ifndef DEFINES_HPP
#define DEFINES_HPP

/// Variables common for both sample progrms

/// How many threads
const size_t N_THREADS = 2;
/// How many elements in a single batch (message)
const size_t BATCH_SIZE = 1024;
/// How many batches do we run
const size_t N_RUNS = 20;
/// Total number of primes to calculate
const size_t N_PRIMES = N_THREADS * BATCH_SIZE * N_RUNS;
/// Artificial slow in the prime calculation function (in milliseconds)
const double DELAY = 1;

#endif  // DEFINES_HPP
