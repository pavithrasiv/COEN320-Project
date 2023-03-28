#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <thread>
#include <iostream>
#include "Timer.h"


using namespace std;

class Plane {
public:
    Plane(int id, int *pos[3], int *vel[3], int time);
    void startThread(int connectionID);
    void stopThread();
    void threadFunction();
    void ThreadStop();
    void ThreadPlaneTimerStart();
    void updatePlaneLocation();
    void InsideAirspace();
    int *getPlaneLocation();

private:
    int aircraftID;
    int *position[3];
    int *velocity[3];
    int arrivalTime;
    bool outsideAirspace;
    thread m_thread;
    bool m_stopThread;
    int m_connectionID;
};

vector<Plane> readPlanesFromFile(string fileName);

#endif // PLANE_H