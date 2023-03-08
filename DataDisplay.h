#ifndef DATADISPLAY_H
#define DATADISPLAY_H
#include <string>

// for one aircraft
typedef struct
{
    int aircraftID;
    Vector3D position; // x y and z components of plane from communication system
    Vector3D velocity;
} singleAircraftDisplay;

// for displaying multiple aircrafts
typedef struct
{
    int numberOfAircrafts;
    Vector3D[numberOfAircrafts] positionArray;
    Vector3D[numberOfAircrafts] velocityArray;
    int[numberOfAircrafts] planeIDArray;
} multipleAircraftDisplay;

union aircraftUnion
// Stored in memory
{
    singleAircraftDisplay single;
    multipleAircraftDisplay multiple;
};

typedef struct
{
    int command; // defining which of union element it is
    aircraftUnion aircraft;

} dataDisplayMessage; // Type of message

class DataDisplay
{
private:
    void run();
    void receiveMessage();
    std::string createGrid(multipleAircraftDisplay &totalAirSaceInfo);

    int channelID;
    int logError;

public:
    DataDisplay();
    int getChannelID() const;
    static void start(void context);
};
#endif