/*
 * Ali Turkman, based on code provided within Timer tutorial
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <errno.h>
#include <sync.h>
#include <unistd.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include <inttypes.h>
#include <stdint.h>

// Complete message passing here


class Timer {
	timer_t timer_id;
	int period;
	int offset;
	struct sigevent sig_event; // For SIGEV_PULSE
	sigset_t sig_set;
	struct itimerspec timer_spec;
	struct timespec tv;



public:
	Timer(int offset, int period);
	void set_timer(int offset, int period);
	void wait_next_activation();
	~Timer();

};

void* TimerThread(void *);


#endif
