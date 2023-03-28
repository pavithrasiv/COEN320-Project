#include "Plane.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#define MIN_AIRSPACE_X_REGION 0
#define MAX_AIRSPACE_X_REGION 100000
#define MIN_AIRSPACE_Y_REGION 0
#define MAX_AIRSPACE_Y_REGION 100000
#define MIN_AIRSPACE_Z_REGION 15000
#define MAX_AIRSPACE_Z_REGION 25000


Plane::Plane(int id, int *pos[3], int *vel[3], int time)
{
    outsideAirspace = false;
    position = pos;
    velocity = vel;
    aircraftID = id;
    arrivalTime = time;
}

void Plane::startThread(int connectionID) {
    m_connectionID = connectionID;
    m_stopThread = false;
    m_thread = thread(&Plane::threadFunction, this);
}

void Plane::stopThread() {
    m_stopThread = true;
    m_thread.join();
}

void Plane::threadFunction() {
    while (!m_stopThread) {
        Message message;
        receiveMessage(m_connectionID, message);
        // Process the message here
    }
}


void Plane::ThreadStop(){
    // Destroy thread here

}

void Plane::ThreadPlaneTimerStart() {
    Timer timer(0, 1); // Add ch and co here
    while (!outsideAirspace) {
        wait_next_activation();
        updatePlaneLocation();
        InsideAirspace();
    }
}

void Plane::updatePlaneLocation()
{
    // Timer trigger int POSITION_UPDATE_TIME = 1;
    position[0] += velocity[0];
    position[1] += velocity[1];
    position[2] += velocity[2];


}

void Plane::InsideAirspace()
{

    // checking to see if left airspace
    if (pos_x < MIN_AIRSPACE_X_REGION || pos_x > MAX_AIRSPACE_X_REGION || pos_y < MIN_AIRSPACE_Y_REGION || pos_y > MAX_AIRSPACE_Y_REGION || pos_z < MAX_AIRSPACE_Z_REGION || pos_z > MAX_AIRSPACE_Z_REGION)
    {
        outsideAirspace = true;
    }
    return outsideAirspace;
}

int *Plane::getPlaneLocation()
{

    return position;
}


vector<Plane> readPlanesFromFile(string fileName) {
    vector<Plane> planes;
    ifstream inputFile(fileName);
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        int id, arrivalTime;
        double xPos, yPos, zPos, xSpeed, ySpeed, zSpeed;
        char comma;
        if (ss >> arrivalTime >> comma >> id >> comma >> xPos >> comma >> yPos >> comma >> zPos >> comma >> xSpeed >> comma >> ySpeed >> comma >> zSpeed) {
            int *pos[3] = {&xPos, &yPos, &zPos};
            int *vel[3] = {&xSpeed, &ySpeed, &zSpeed};
            planes.emplace_back(id, pos, vel, arrivalTime);
        }
    }
    return planes;
}
