/**
*   @author Joonatan Kuosa <joonatan.kuosa@gmail.com>
*   @date 2018-05
*   @file main.cpp
*
*   Under a copyleft.
*/

#include "sleep.hpp"
#include "chrono.hpp"

#include <thread>
#include <vector>
#include <cassert>
#include <fstream>
#include <sstream>

#include "fifo.hpp"
#include "prime.hpp"
#include "defines.hpp"

// Valid message types (well 0 is not valid)
const uint16_t MSG_UNDEFINED = 0;
const uint16_t MSG_BATCH = 1;
const uint16_t MSG_RESULTS = 2;
const uint16_t MSG_EXIT = 3;

// @todo for large batches we need to switch to dynamic memory
struct Message
{
    Message(uint16_t type) : msg(type), size(0) {}
    Message() : msg(MSG_UNDEFINED), size(0) {}

    uint16_t msg;
    size_t data[BATCH_SIZE];
    size_t size;
};

#define buffer_t fifo<Message>

// worker function
void primes(buffer_t *in, buffer_t *out)
{
    bool cont = true;
    while (cont)
    {
        if (!in->empty())
        {
            auto data = in->pop();
            switch(data.msg)
            {
            case MSG_BATCH:
            {
                Message msg(MSG_RESULTS);
                for(size_t i = 0; i < data.size; ++i)
                {
                    really_slow_func(DELAY);
                    auto n = data.data[i];
                    if(isPrime(n))
                    {
                        msg.data[msg.size] = n;
                        ++msg.size;
                    }
                }
                out->push(msg);
            }
            break;
            case MSG_EXIT:
            {
                cont = false;
            }
            break;
            default:
                // just ignore
                break;
            }
        }
    }
}

/// @brief read data from threads and print it to standard out
/// @param in buffers for all threads (an array)
/// @param n_threads how many threads
/// @param n_rec OUT how many responses have we got
/// @param c_primes OUT how many primes we found so far
void read_from_threads(buffer_t *in, const size_t n_threads, size_t &n_rec, size_t &c_primes)
{
    for (size_t i = 0; i < n_threads; ++i)
    {
        while (!in[i].empty())
        {
            ++n_rec;
            auto data = in[i].pop();
            for(size_t j = 0; j < data.size; ++j)
            {
                std::cout << data.data[j] << " is a prime (thread: " << i << ")" << std::endl;
                ++c_primes;
            }
        }
    }
}


int main(int argc, char *argv[])
{

    // Redirect cout
    // simpler to print into it, but console is slow as sin
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ofstream fout("output_multi_t.txt");
    std::cout.rdbuf(fout.rdbuf());

    // print out the starting parameters
    std::stringstream ss;
    ss << "Starting with " << N_THREADS << " threads : " << BATCH_SIZE << " per batch : "
        << N_RUNS << " batches." << std::endl
        << " Checking " << N_PRIMES << " numbers for prime number." << std::endl
        << " With a delay of " << DELAY << "ms per function call.";
    std::cout << ss.str() << std::endl;
    std::clog << ss.str() << std::endl;

    // full application clock
    vl::chrono app_timer;

    buffer_t out[N_THREADS];
    buffer_t in[N_THREADS];
    std::vector<std::thread> workers;

    auto clock = vl::chrono();
    // spawn threads
    for (size_t i = 0; i < N_THREADS; ++i)
    {
        workers.push_back(std::thread(primes, &out[i], &in[i]));
    }

    std::cout << "Took " << clock.elapsed() << " to create workers." << std::endl;

    // push data to threads
    // we keep count because we want to continue in the next phase
    size_t count = 0;   // how many numbers so far
    size_t c_primes = 0;// how many primes so far
    size_t n_sent = 0;  // how many messages have we sent
    size_t n_rec = 0;   // how many messages have we received
    for (size_t run = 0; run < N_RUNS; ++run)
    {
        std::cout << "Push Data" << std::endl;
        clock.reset();
        for (size_t i = 0; i < N_THREADS; ++i)
        {
            ++n_sent;
            Message msg(MSG_BATCH);
            msg.size = BATCH_SIZE;
            for (size_t j = 0; j < BATCH_SIZE; ++j)
            {
                // push to the thread
                msg.data[j] = count;
                ++count;
            }
            out[i].push(msg);
        }
        std::cout << run << " : Took " << clock.elapsed() << " to push data." << std::endl;

        // sleep to force a context switch (so our workers get going)
        vl::sleep(1);

        std::cout << "Pull data" << std::endl;
        clock.reset();

        read_from_threads(in, N_THREADS, n_rec, c_primes);

        std::cout << run << " : Took " << clock.elapsed() << " to get data." << std::endl;
    }

    clock.reset();
    // Wait for data, we should have same amount of messages in each direction
    while(n_sent != n_rec)
    {
        read_from_threads(in, N_THREADS, n_rec, c_primes);

        vl::sleep(1);
    }
    std::cout << "Took " << clock.elapsed() << " to wait for all the data." << std::endl;

    // Final reports to console and file
    ss.str("");
    ss << "ALL DONE" << std::endl
        << " found " << c_primes << " prime numbers."
        << " from " << N_RUNS * BATCH_SIZE << std::endl
        << "Total time: " << app_timer.elapsed();
    std::cout << ss.str() << std::endl;
    std::clog << ss.str() << std::endl;

    // Cleanup
    for (size_t i = 0; i < N_THREADS; ++i)
    {
        Message msg(MSG_EXIT);
        out[i].push(msg);
    }

    for(size_t i = 0; i < N_THREADS; ++i)
    {
        workers.at(i).join();
    }

    return 0;
}
