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

set(HEADERS
    sleep.hpp
    time.hpp
    chrono.hpp 
    )

set(SRC
    time.cpp
	chrono.cpp
	primes_threaded.cpp
    )

if(WIN32)
	message(STATUS "Using multiple cores for building")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

add_executable(test_fifo
	test_fifo.cpp)

add_executable(primes_threaded
	${SRC}
	${HEADERS}
	${BASE_SRC}
    )

target_link_libraries(message_passing ${DEP_LIBS})
