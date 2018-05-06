/**
 *  Copyright (c) 2010 - 2011 Tampere University of Technology
 *  Copyright (c) 2011 - 2014 Savant Simulators
 *
 *  @author Joonatan Kuosa <joonatan.kuosa@savantsimulators.com>
 *  @date 2011-07
 *  @file base/sleep.hpp
 *
 *  This file is part of Hydra VR game engine.
 *  Version 0.5
 *
 *  MIT license (like the orginal TUT version)
 *
 */

#ifndef HYDRA_BASE_SLEEP_HPP
#define HYDRA_BASE_SLEEP_HPP

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#include "time.hpp"

namespace vl
{

// Forbid implicit conversions
// primitive types allow conversions but overloads take precidence
// over implicit conversion so this function is used if user does
// not use explicit conversions.
template <typename T>
inline void msleep(T) = delete;

inline void msleep(uint32_t milliseconds)
{
#ifdef _WIN32
    ::Sleep(milliseconds);
#else   // _WIN32
    timespec tv;
    tv.tv_sec = 0;
    tv.tv_nsec = milliseconds * 1e6;
    ::nanosleep( &tv, 0 );
#endif  // _WIN32
}

inline void sleep(vl::time const &t)
{
    double ms = ((double)t)*1e3;
    msleep((uint32_t)ms);
}

}   // namespace vl

#endif  // HYDRA_BASE_SLEEP_HPP
