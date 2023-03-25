
#include "Structure.h"

#include <pthread.h>

int calculationInterval;

// computer system thread
pthread_t CompSystemThread();

// thread body
void * CompSystemMain( void * arg);
