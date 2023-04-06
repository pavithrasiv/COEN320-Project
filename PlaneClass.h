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

using namespace std;

class PlaneClass {
public:
    PlaneClass(int id, int posX, int posY, int posZ, int velX, int velY, int velZ, int time);
    void ThreadPlaneTimerStart();
    void update();
    bool insideAirspace();
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
    bool outsideAirspace;
};

vector<PlaneClass> readPlanesFromFile(string fileName);

#endif // PLANE_H