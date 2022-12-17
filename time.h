#ifndef _TIME_H_
#define _TIME_H_

// ##### get_time () implementation #####

#ifdef _WIN32
#include <windows.h>

double get_time () {
    FILETIME systime ;
    ULARGE_INTEGER utime ;
    GetSystemTimeAsFi leTime (& systime ) ; // resolution 100 nanoseconds
    utime . LowPart = systime . dwLowDateTime ;
    utime . HighPart = systime . dwHighDateTime ;

    return utime . QuadPart / 10000000.0;
}

#else
#include <sys/time.h>

double get_time () {
    struct timeval tv ;
    gettimeofday (& tv , NULL ) ;
    return tv.tv_sec + ( tv.tv_usec ) /1000000.0;
}

#endif
#endif