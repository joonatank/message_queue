#-------------------------------------------------------------------
# Joonatan Kuosa
# 2018-05
#
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#
#-------------------------------------------------------------------

project( "MessagePassing" CXX )

cmake_minimum_required(VERSION 2.6)

if(WIN32)
    message(STATUS "Using multiple cores for building")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

add_executable(test_fifo
    test_fifo.cpp)

add_executable(primes_threaded
    time.cpp
    chrono.cpp
    primes_threaded.cpp
    )

add_executable(primes_reference
    primes_reference.cpp
    chrono.cpp
    time.cpp
    )
