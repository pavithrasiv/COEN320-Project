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

Plane::Plane(int id, int pos[3], int vel[3], int time)
{
    outsideAirspace = false;
    for (int i = 0; i < 3; i++)
    {
        position[i] = pos[i];
        velocity[i] = vel[i];
    }
    aircraftID = id;
    arrivalTime = time;
    m_connectionID = -1;
}
int Plane::getChannelID()
{
    return m_connectionID;
}

void Plane::startThread(int connectionID)
{
    m_connectionID = ChannelCreate(0);
    if (m_connectionID == -1)
    {
        cout << "Error could not create Channel" << endl;
    }
    else
    {
        m_stopThread = false;
        m_thread = thread(&Plane::threadFunction, this);
    }
}

void Plane::stopThread()
{
    m_stopThread = true;
    m_thread.join();
}

void Plane::threadFunction()
{
    while (!m_stopThread)
    {
        Message message;
        receiveMessage(m_connectionID, message);
        // Process the message here
    }
}

void Plane::ThreadStop()
{
    // Destroy thread here
}

void Plane::ThreadPlaneTimerStart()
{
    Timer timer(0, 1); // Add ch and co here
    while (!outsideAirspace)
    {
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
    if (position[0] < MIN_AIRSPACE_X_REGION || position[0] > MAX_AIRSPACE_X_REGION || position[1] < MIN_AIRSPACE_Y_REGION || position[1] > MAX_AIRSPACE_Y_REGION || position[2] < MAX_AIRSPACE_Z_REGION || position[2] > MAX_AIRSPACE_Z_REGION)
    {
        outsideAirspace = true;
    }
    return outsideAirspace;
}

int *Plane::getPlaneLocation()
{

    return position;
}
int *Plane::getPlaneVelocity()
{
    return velocity;
}

vector<Plane> readPlanesFromFile(string fileName)
{
    vector<Plane> planes;
    ifstream inputFile(fileName);
    string line;
    while (getline(inputFile, line))
    {
        stringstream ss(line);
        int id, arrivalTime;
        int xPos, yPos, zPos, xSpeed, ySpeed, zSpeed;
        char comma;
        if (ss >> arrivalTime >> comma >> id >> comma >> xPos >> comma >> yPos >> comma >> zPos >> comma >> xSpeed >> comma >> ySpeed >> comma >> zSpeed)
        {
            position[3] = {xPos, yPos, zPos};
            velocity[3] = {xSpeed, ySpeed, zSpeed};
            planes.emplace_back(id, pos, vel, arrivalTime);
        }
    }
    return planes;
}
