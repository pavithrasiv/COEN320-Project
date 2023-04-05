#include <pthread.h>

pthread_t createCommunication();

void * communicationMain();

void sendMessageToAirplane(const char* planeChannel, Msg2Airplane message);
