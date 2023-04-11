#include "Timer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#define ATTACH_POINT "my_channel"
using namespace std;
#include "PlaneClass.h"

// Ali Turkman 40111059

//Message header is at least a pulse
//This identifies the message that is being sent and received
//Unique ID for it basically
typedef struct _pulse msg_header_t;

//Format of plane
typedef struct _time_data {
	msg_header_t hdr;
	int time;
} time_data_t;



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

void* TimerThread(void *) {
	time_data_t time_msg;

	int planeclient_coid;
	if ((planeclient_coid = name_open(ATTACH_POINT, 0)) == -1) {
				perror("Error occurred while attaching the channel");
				}

	uint32_t period_sec=1;
	double period=period_sec;
	Timer timer(period_sec,period_sec);
	int count=0;
	double t = 0;
	time_msg.hdr.type=0x00;
	time_msg.hdr.subtype=0x01;
	while(t<30.0){
		t=count*period;
		time_msg.time = t;
		if (MsgSend(planeclient_coid, &time_msg, sizeof(time_msg), NULL,0) == -1){
							    	perror("Error sending speed message");
							    	break;
							    		}
		timer.wait_next_activation();
		count++;
		}


}


Timer::~Timer() {
	// TODO Auto-generated destructor stub
}


int mainz(int argc, char*argv[]){

	//Declare thread
		pthread_t timerreceiver_thread;
		int err_no;
		err_no = pthread_create(&timerreceiver_thread, NULL, &TimerReceiverThread, NULL);
		if (err_no != 0){
			perror("Error creating server thread! \n");
		}

		pthread_t timersender_thread;
		err_no = pthread_create(&timersender_thread, NULL, &TimerThread, NULL);
		if (err_no != 0){
			perror("Error creating client thread! \n");
		}

		//Join thread to start it
		err_no = pthread_join(timerreceiver_thread, NULL);
		if (err_no != 0){
			perror("Error joining server thread!");
		}

		err_no = pthread_join(timersender_thread, NULL);
		if (err_no != 0){
			perror("Error joining client thread!");
		}
		pthread_exit(EXIT_SUCCESS);
		return 0;
}
