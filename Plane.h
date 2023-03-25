#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <thread>
#include <iostream>
#include "Timer.h"

class Plane {
public:
    // Constructor
    Plane(string id, double arrivalTime, double xPos, double yPos, double zPos, double xSpeed, double ySpeed, double zSpeed);
    
    // Getters
    string getID() const;
    double getArrivalTime() const;
    double getXPos() const;
    double getYPos() const;
    double getZPos() const;
    double getXSpeed() const;
    double getYSpeed() const;
    double getZSpeed() const;

    // Setters
    void setID(string id);
    void setArrivalTime(double arrivalTime);
    void setXPos(double xPos);
    void setYPos(double yPos);
    void setZPos(double zPos);
    void setXSpeed(double xSpeed);
    void setYSpeed(double ySpeed);
    void setZSpeed(double zSpeed);

    // Thread functions
    void startThread(int connectionID);
    void stopThread();

    ~Plane();

private:
    string m_id;
    double m_arrivalTime;
    double m_xPos;
    double m_yPos;
    double m_zPos;
    double m_xSpeed;
    double m_ySpeed;
    double m_zSpeed;

    // Thread data
    thread m_thread;
    bool m_stopThread;
    int m_connectionID;

    // Thread functions
    void threadFunction();
};

#endif // PLANE_H
