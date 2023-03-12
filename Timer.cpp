#include "Timer.h"

Timer::Timer(int offset, int period) {

	const int signal = SIGALRM;
	sigemptyset(&sig_set); // initialize a signal set
	sigaddset(&sig_set, signal); // add SIGALRM to the signal set
	sigprocmask(SIG_BLOCK, &sig_set, NULL); //block the signal


	/* set the signal event a timer expiration */
	memset(&sig_event, 0, sizeof(struct sigevent));
	sig_event.sigev_notify = SIGEV_SIGNAL;
	sig_event.sigev_signo = signal;
	CHID = ChannelCreate(0);
	COID = ConnectAttach(0, 0, CHID, 0, 0);
	SIGEV_PULSE_INIT(&event, COID, SIGEV_PULSE_PRIO_INHERIT, 1, 0);



	if (timer_create(CLOCK_REALTIME, &sig_event, &timer_id) == -1){
		std::cerr << "Timer had an error! Error #" << errno << "!\n";
	}

	set_timer(offset, period);

}

// Good
Timer::set_timer(int offset, int period){
	timer_spec.it_value.tv_sec = offset;
	timer_spec.it_value.tv_nsec = 0;
	timer_spec.it_interval.tv_sec = period;
	timer_spec.it_interval.tv_nsec = 0;
	timer_settime(timer_id, 0, &timer_spec, NULL);
	std::cout << "Timer has started!";
}

void Timer::wait_next_activation() {
	int dummy;
	/* suspend calling process until a signal is pending */
	sigwait(&sig_set, &dummy);
}

