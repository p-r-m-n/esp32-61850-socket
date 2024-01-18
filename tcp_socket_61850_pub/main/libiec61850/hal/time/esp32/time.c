/*
 *  time.c
 *
 *  Copyright 2013-2021 Michael Zillgith
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850, libmms, and lib60870.
 */

#include "hal_time.h"
#include <time.h>
#include "time.h"
#include "pthread.h"
#include <esp_pthread.h>
#include "esp_sntp.h"

#ifdef CONFIG_SYSTEM_HAS_gettimeofday
uint64_t
Hal_getTimeInMs()
{
	struct timespec tp;

	gettimeofday(CLOCK_REALTIME, &tp);

	return ((uint64_t) tp.tv_sec) * 1000LL + (tp.tv_nsec / 1000000);
}
#else




msSinceEpoch
Hal_getTimeInMs()
{
    struct timeval now;

    gettimeofday(&now, NULL);

    return ((uint64_t) now.tv_sec * 1000LL) + (now.tv_usec / 1000);
}

nsSinceEpoch
Hal_getTimeInNs()
{
    struct timespec now;

    gettimeofday(CLOCK_REALTIME, &now);

    nsSinceEpoch nsTime = now.tv_sec * 1000000000UL;
    nsTime += now.tv_nsec;

    return nsTime;
}

bool
Hal_setTimeInNs(nsSinceEpoch nsTime)
{
    struct timespec tv;

    tv.tv_sec = nsTime / 1000000000UL;
    tv.tv_nsec = nsTime % 1000000000UL;

    if (clock_settime(CLOCK_REALTIME, &tv) < 0) {
        return false;
    }

    return true;
}


#endif

