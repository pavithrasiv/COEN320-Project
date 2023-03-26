#include "Plane.h"
#include <vector>
#define MIN_AIRSPACE_X_REGION 0
#define MAX_AIRSPACE_X_REGION 100000
#define MIN_AIRSPACE_Y_REGION 0
#define MAX_AIRSPACE_Y_REGION 100000
#define MIN_AIRSPACE_Z_REGION 15000
#define MAX_AIRSPACE_Z_REGION 25000

Plane::Plane()
{
    outsideAirspace = false;
}
Plane::Plane(std::vector<int> pos, std::vector<int> vel, int id, int time)
{
    outsideAirspace = false;
    position = pos;
    velocity = vel;
    aircraftID = id;
    arrivalTime = time;
}

void Plane::updatePlaneLocation()
{
    int POSITION_UPDATE_TIME = 1;
    position[0] += velocity[0] * POSITION_UPDATE_TIME;
    pos_y += vel_y * POSITION_UPDATE_TIME;
    pos_z += vel_z * POSITION_UPDATE_TIME;
    // checking to see if left airspace
    if (pos_x < MIN_AIRSPACE_X_REGION || pos_x > MAX_AIRSPACE_X_REGION || pos_y < MIN_AIRSPACE_Y_REGION || pos_x > MAX_AIRSPACE_Y_REGION || pos_z < MAX_AIRSPACE_Z_REGION || pos_z > MAX_AIRSPACE_Z_REGION)
    {
        outsideAirspace = true;
        pos_x = -1;
        pos_y = -1;
        pos_z = -1;
    }
}

int *Plane::getPlaneLocation()
{

    return position;
}