#include "Plane.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Plane thread 

// Plane IPC


// Plane constructor
Plane::Plane(string id, double arrivalTime, double xPos, double yPos, double zPos, double xSpeed, double ySpeed, double zSpeed)
    : m_id(id), m_arrivalTime(arrivalTime), m_xPos(xPos), m_yPos(yPos), m_zPos(zPos), m_xSpeed(xSpeed), m_ySpeed(ySpeed), m_zSpeed(zSpeed)
{
}

string Plane::getID() const {
    return m_id;
}

double Plane::getArrivalTime() const {
    return m_arrivalTime;
}

double Plane::getXPos() const {
    return m_xPos;
}

double Plane::getYPos() const {
    return m_yPos;
}

double Plane::getZPos() const {
    return m_zPos;
}

double Plane::getXSpeed() const {
    return m_xSpeed;
}

double Plane::getYSpeed() const {
    return m_ySpeed;
}

double Plane::getZSpeed() const {
    return m_zSpeed;
}

void Plane::setID(string id) {
    m_id = id;
}

void Plane::setArrivalTime(double arrivalTime) {
    m_arrivalTime = arrivalTime;
}

void Plane::setXPos(double xPos) {
    m_xPos = xPos;
}

void Plane::setYPos(double yPos) {
    m_yPos = yPos;
}

void Plane::setZPos(double zPos) {
    m_zPos = zPos;
}

void Plane::setXSpeed(double xSpeed) {
    m_xSpeed = xSpeed;
}

void Plane::setYSpeed(double ySpeed) {
    m_ySpeed = ySpeed;
}

void Plane::setZSpeed(double zSpeed) {
    m_zSpeed = zSpeed;
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

// Plane output 
vector<Plane> readPlanesFromFile(string fileName) {
    vector<Plane> planes;
    ifstream inputFile(fileName);
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string id;
        double arrivalTime, xPos, yPos, zPos, xSpeed, ySpeed, zSpeed;
        char comma;
        if (ss >> arrivalTime >> comma >> id >> comma >> xPos >> comma >> yPos >> comma >> zPos >> comma >> xSpeed >> comma >> ySpeed >> comma >> zSpeed) {
            planes.emplace_back(id, arrivalTime, xPos, yPos, zPos, xSpeed, ySpeed, zSpeed);
        }
    }
    return planes;
}