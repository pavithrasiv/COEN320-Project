#include "Timer.h"

Timer::Timer(int offset, int period) {

	const int signal = SIGALRM;
	sigemptyset(&sig_set);
	sigaddset(&sig_set, signal);
	sigprocmask(SIG_BLOCK, &sig_set, NULL);



	memset(&sig_event, 0, sizeof(struct sigevent));
	sig_event.sigev_notify = SIGEV_SIGNAL;
	sig_event.sigev_signo = signal;




	if (timer_create(CLOCK_REALTIME, &sig_event, &timer_id) == -1){
		std::cerr << "Timer had an error! Error #" << errno << "!\n";
	}

	set_timer(offset, period);

}

// Good
void Timer::set_timer(int offset, int period){
	timer_spec.it_value.tv_sec = offset;
	timer_spec.it_value.tv_nsec = 0;
	timer_spec.it_interval.tv_sec = period;
	timer_spec.it_interval.tv_nsec = 0;
	timer_settime(timer_id, 0, &timer_spec, NULL);
	std::cout << "Timer has started! \n";
}

void Timer::wait_next_activation() {
	int dummy;
	/* suspend calling process until a signal is pending */
	sigwait(&sig_set, &dummy);
}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}
