#ifndef PLANE_H
#define PLANE_H

#include <vector>

class Plane
{
public:
    Plane();
    Plane(int *pos[3], int *vel[3], int, int);
    int *getPlaneLocation();
    void updatePlaneLocation();

private:
    int *position[3];
    int *velocity[3];
    int arrivalTime;
    int aircraftID;
    bool outsideAirspace;
};
#endif