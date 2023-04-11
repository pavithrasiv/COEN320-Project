#include "DataDisplay.h"
#include <pthread.h>

int calculationInterval;

// computer system thread
pthread_t CompSystemThread();

// thread body
void * CompSystemMain( void * arg);

// Functions to change parameters
void changeSpeed(int id, double s);
void changeAltitude(int id, int a);
void changePosition(int id, float orient);
