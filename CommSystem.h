#include <pthread.h>
#include <vector.h>

pthread_t createCommunication();

void * communicationMain();

void sendMessage(int chid, const void *msg, int size);

void disconnectFromChannel(int coid);

CommSystem (vector[] p);