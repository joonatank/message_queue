/**
*	@author Joonatan Kuosa <joonatan.kuosa@gmail.com>
*	@date 2018-05
*	@file main.cpp
*
*	Under a copyleft.
*/

#include "sleep.hpp"
#include "chrono.hpp"

#include <thread>
#include <vector>
#include <cassert>
#include <fstream>

#include "fifo.hpp"
#include "prime.hpp"

const size_t N_THREADS = 4;
const size_t BATCH_SIZE = 1024;
const size_t N_RUNS = 20;

// Valid message types (well 0 is not valid)
const uint16_t MSG_UNDEFINED = 0;
const uint16_t MSG_BATCH = 1;
const uint16_t MSG_EXIT = 2;

struct Message
{
	uint16_t msg;
	int data[BATCH_SIZE];
};

// In a real application we would use a batch size field of ints for example 1024
// and push a full batch in one go.
// But this illustrates the system for a generic message typy.
#define buffer_t fifo<size_t>



// worker function
void primes(buffer_t *in, buffer_t *out)
{
	while (1)
	{
		if (!in->empty())
		{
			//std::clog << "Worker: getting data" << std::endl;
			auto data = in->pop();
			if(isPrime(data))
			{
				out->push(data);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	// Redirect cout
    // simpler to print into it, but console is slow as sin
	std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
	std::ofstream fout("output.txt");
	std::cout.rdbuf(fout.rdbuf());

    // print out the starting parameters
    std::cout << N_THREADS << " threads : " << BATCH_SIZE << " per batch : "
        << N_RUNS << " batches." << std::endl;
    // full application clock
    vl::chrono app_timer;

	// Spawn four threads
	// create deques for all of them
	// input -> output
	// in_fifo[thread_n]
	// out_fifo[thread_n]
	// thread reads from out, writes into in (named from master)
	// we collect all data here

	// create a test data of lets say 10000 numbers
	// split it into N_THREAD slices
	// pass it to the threads
	// check if the number is a prime or not (use the algo in Wikipedia)

	// pass the primes back to main thread
	
	buffer_t out[N_THREADS];
	buffer_t in[N_THREADS];
	std::vector<std::thread> workers;

	auto clock = vl::chrono();
	// spawn
	for (size_t i = 0; i < N_THREADS; ++i)
	{
		workers.push_back(std::thread(primes, &out[i], &in[i]));
	}

	std::cout << "Took " << clock.elapsed() << " to create workers." << std::endl;

	// push data to threads
	// we keep count because we want to continue in the next phase
	size_t count = 0;
	size_t c_primes = 0;
	for (size_t run = 0; run < N_RUNS; ++run)
	{
		std::cout << "Push Data" << std::endl;
		clock.reset();
		for (size_t i = 0; i < N_THREADS; ++i)
		{
			size_t N = count + BATCH_SIZE;
			std::cout << "thread: " << i << " start data: " << count << std::endl;
			for (size_t j = count; j < N; ++j)
			{
				// test printing
				//std::cout << "thread: " << i << " data: " << j << std::endl;
				// push to the thread
				out[i].push(j);
				++count;
			}
		}
		std::cout << run << " : Took " << clock.elapsed() << " to push data." << std::endl;

		vl::sleep(1);

		std::cout << "Pull data" << std::endl;
		clock.reset();
		// read the results (for now print to console)
		// later we time this and print to file
		for (size_t i = 0; i < N_THREADS; ++i)
		{
			std::cout << "From thread: " << i << std::endl;
			while (!in[i].empty())
			{
				auto data = in[i].pop();
				std::cout << "thread: " << i << " received: " << data << std::endl;
                ++c_primes;
			}
		}
		std::cout << run << " : Took " << clock.elapsed() << " to get data." << std::endl;

		// push more data
	}

	// Cleanup
	std::cout << "ALL DONE" << " found " << c_primes << " prime numbers."
        << " from " << N_RUNS * BATCH_SIZE << std::endl;
    std::cout << "Total time: " << app_timer.elapsed() << std::endl;

	// this does not cleanup... (terminate threads) we need to manually kill them
	// which is not part of the c++ standard
	for (size_t i = 0; i < N_THREADS; ++i)
	{
		bool ret = TerminateThread((HANDLE)workers.back().native_handle(), 0);
		assert(ret);
		workers.pop_back();
	}
	
}