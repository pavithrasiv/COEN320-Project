#include <iostream>
#include "CommSystem.h"
#include "CompSystem.h"
#include "Radar.h"
#include "Timer.h"
#include "Plane.h"

using namespace std;

int main(){
    
//initialize the p_threads here 

	pthread_t CommSystemThread = createCommunicationThread();





	pthread_join(CommSystemThread, NULL);


    return 0;
}