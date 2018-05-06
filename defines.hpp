/**
*	@author Joonatan Kuosa <joonatan.kuosa@gmail.com>
*	@date 2018-05
*	@file defines.hpp
*
*	Under a copyleft.
*/

#ifndef DEFINES_HPP
#define DEFINES_HPP

const size_t N_THREADS = 2;
const size_t BATCH_SIZE = 1024;
const size_t N_RUNS = 20;

const size_t N_PRIMES = N_THREADS * BATCH_SIZE * N_RUNS;

const double DELAY = 1;

#endif  // DEFINES_HPP