# Non locking message queue
Sample of a message queue that can be used for non-locking synchronisation in multi threading.

## Use cases
1. Where the state initialisation requires a lot of time so spawning a new worker threads isn't an option.
2. For background tasks when they need state data (especially persistent state).
3. Not a fan of locking - deadlocks and other problems because of bad architecture or programming errors.
4. Where you need to run different functions on a separate context with it's own state.
5. Where you need to occasionally pass data between two separate threads.

We used similar system in our rendering engine, where one thread (computer) drives the process and other's handled rendering (normally we had 3-5 renderers). Though ours used UDP for communications in the end since we needed separate computers not just threads.

Shared resources were not an option since all the renderers needed their own state data, limitation of OpenGL: it modifies the state during rendering and you can't duplicate state changes across threads.
Our state setup times were a few hundred seconds, while update times were around 1ms (10-100 times a second).

All of our resources (state) used for rendering were duplicated on all threads, we would send the delta (change) since last update to every thread.
Anybody who missed an update for any reason would get a list of deltas and apply those in order.

## Limitations
* Overhead for every message: you need to allocate and copy data every time you pass data between threads.
* Needlessly complex for background threads like file loading: a single callback function is much simpler.
* Not the best solution for data parallel problems e.g. prime calculation, linear algebra, data analytics, image processing.

For data parallel problems it's more efficient to use shared memory that is sliced for each thread. You need to be careful that you don't access somebody elses slice or that the slices are read-only. You still need barriers when the next function requires the previous data, but it avoids a major problem with message queues which is the copying of data and unnecessary allocations.

## How it works
We use Linked list to implement the FIFO so only two elements are ever accessed by one operation.
* thread 1 reads from FIFO
* thread 2 writes to FIFO

Since writes access only the back of the buffer and reads only the front they never collide.

We need to use one atomic variable for the pointer that is used for reading to handle cases where the buffer is empty. Atomic variables aren't locked but retrieval/assigment is guaranteed to be a single thread safe operation.

We use two fifos for every thread one the worker can read and the other it can write.

### Picture of the FIFOs
![alt text](worker_fifos.svg "FIFOs for four worker threads.")


More info: [Dr. Dobbs article on lock-free Message Queues](http://www.drdobbs.com/parallel/writing-lock-free-code-a-corrected-queue/210604448#)

## Sample
The sample has a few simple message types used to calculate prime numbers in separate threads.

1. The main thread initialises data.
2. Sends it over to workers as batches of 1024 elements.
3. The workers tests for a prime number for all those elements.
4. Optional: We introduce a delay in each calculation (to simulate a more complex function)
5. Send back the numbers that were primes.

The sample code doesn't model the use case very well since it's a data parallel problem.
Making a proper use case requires adding more complexity: state initialisation, complex functions or a lot of different functions that can be executed on a separate thread.

To illustrate performance gains (even in data parallel applications) I added a delay parameter that simulates a more complex function call used instead of a prime checking.

Performance gains:
* Negative without a delay (as expected since prime checking is fast, we'd need to use huge data buffers)
* With 0.5ms delay on two cores as fast as the single threaded version.
* With 1ms delay on two cores 64% faster than single threaded version (25s vs 41s with 20k function calls, 20 batches 1024 per batch) on an i3 laptop using x64 release.

## Code
* primes_threaded.cpp - main application for the message queue version
* primes_reference.cpp - main application for the reference (single thread)

* prime.hpp - contains the functions used by both
* fifo.hpp - contains the thread safe message queue
* defines.hpp - contains parameters for the program (how many threads, batch size etc.)

* test_fifo.cpp - contains unit tests for fifo

utility:
* chrono.cpp, chrono.hpp - counters for checking performance
* time.cpp, time.hpp     - time data type
* sleep.hpp              - as always multithreading is exhausting


## Compile
* Compiles on MSVC 14.1 (2017) at least, Linux or GCC not tested.
* Requires CMake (either stand-alone or Visual Studio plugin)
* Requires C++11

Doesn't require any external libraries just standard library and Win32 (System lib on Linux).

To compille: either open in Visual Studio (CMake plugin) or run CMake in the source tree

## Running
Prameters:
* {N_THREADS} - Number of threads (for single threaded changes the number of primes to calculate)
* {DELAY} - Artificial delay in function calls (milliseconds)
* {OUTPUT_FILENAME} - log file name

#### primes_reference - single threaded version
primes_reference.exe {N_THREADS} {DELAY} {OUTPUT_FILENAME}

example (default arguments):

primes_reference.exe 2 1 output_single_t.txt


#### primes_threaded - multi-threaded version
primes_reference.exe {N_THREADS} {DELAY} {OUTPUT_FILENAME}

example (default arguments):

primes_threaded.exe 2 1 output_multi_t.txt

#### Default Parameters
The default parameter values are in defines.hpp:
* N_THREADS - How many threads
* BATCH_SIZE - How many numbers per one message
* N_RUNS - How many batches (messages) we send total
* DELAY - Artificial slow in the function call in milliseconds

### TODO
Run a proper tests with 2, 4, 8, 16, 32 threads and document. Seems like same execution time with all of those even though the data amount is doubled and it introduces a lot of context switching. Why?
