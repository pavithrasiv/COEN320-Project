#ifndef PLANECLASS_H
#define PLANECLASS_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include "Timer.h"
#include <vector>
#include <string>
#include <sys/neutrino.h>

// Ali Turkman 40111059

using namespace std;

class PlaneClass {
public:
    PlaneClass(int id, int posX, int posY, int posZ, int velX, int velY, int velZ, int time);
    void ThreadPlaneTimerStart();
    void update();
    bool outsideAirspace();
    int getAircraftID() const;
    int getArrivalTime() const;
    int getPosition(int posValue) const;
    int getVelocity(int posValue) const;
    ~PlaneClass();

private:
    int aircraftID;
    int positionTrue[3];
    int velocity[3];
    int arrivalTime;
    bool outsideAirspaceStatus;
};

vector<PlaneClass> readPlanesFromFile(string fileName);
void* PlaneClientThread(void *);
void* TimerReceiverThread(void *);
void* PlaneClientTestThread(void *);
#endif // PLANE_H

