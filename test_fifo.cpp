/**
*   @author Joonatan Kuosa <joonatan.kuosa@gmail.com>
*   @date 2018-05
*   @file test_fifo.cpp
*
*   Under a copyleft.
*/

#include "fifo.hpp"

#include <iostream>

template<typename T>
void check(T a, T b, const char *msg)
{
    if (!(a == b))
    {
        std::cerr << "TEST FAILED : " << msg << std::endl;
    }
}

int main(int argc, char **argv)
{
    std::cout << "STARTING fifo test" << std::endl;

    fifo<int> fifo;

    fifo.push(1);
    fifo.push(2);
    fifo.push(3);
    fifo.push(4);

    check(fifo.pop(), 1, "fifo pop");
    check(fifo.pop(), 2, "fifo pop");
    check(fifo.pop(), 3, "fifo pop");
    check(fifo.empty(), false, "fifo empty");
    check(fifo.pop(), 4, "fifo pop");
    check(fifo.empty(), true, "fifo not empty");

    std::cout << "fifo test ENDED" << std::endl;
}
