#include "DataDisplay.h"
#include <sys/neutrino.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <iostream>
#define ROWSIZE 100
#define COLUMSIZE 100
#define CELLSIZE 25

#define SINGLE_PLANE 1
#define GRID 3
#define LOG 4
DataDisplay::DataDisplay() : channelID(-1) fd(-1)
{
}

int DataDisplay::getChannelID() const
{
    return channelID;
}

void DataDisplay::run()
{
    channelID = channelCreate(0);
    if (channelID == -1)
    {
        std::cout << "Channel failed to create. Exiting thread." << std::endl;
        return;
    }
    
    fd = creat( "/data/home/qnxuser/myfile.dat", S_IRUSR | S_IWUSR | S_IXUSR );
    if (fd == -1){
        cout << "Log file could not be created" << endl;
    }
    receiveMessage(); // start to listen for messages
}

void DataDisplay::receiveMessage()
{
    int receiveMessageID;   // receive id
     //
    while (1)
    {

        Compsystem command;
        receiveMessageID = MsgReceive(channelID, &command, sizeof(command), NULL); // get id for that specific message

        switch (command)
        {
        case SINGLE_PLANE:
        {
            // Command to display Warning of one plane
            MsgReply(receiveMessageID, EOK, NULL, 0); // sending basic ACK
            std::cout << "WARNING: Aircraft ID: " << msg.getAircraftID()
                      << std::endl
                      << "Aircraft position: "
                      << msg.getPlaneLocation()
                      << std::endl
                      << "Aircraft velocity " << msg.getPlaneVelocity() << "is on a TRAJECTORY TO CRASH!"
                      << std::endl;
            break;
        }
        case GRID: // Top View grid (X and Y)
        {
            // Command to print a grid to the console
            // The sender deletes the arrays allocated for grid printing once we reply, so we need them to stay valid until then.
            std::cout << generateGrid(planes);
            MsgReply(receiveMessageID, EOK, NULL, 0);
            break;
        }
        case LOG:
        {
            // Command to print a grid to the log file
            char buff[] = generateGrid(planes);
            MsgReply(rcvid, EOK, NULL, 0);
            size_written = write( fd, buff, sizeof( buff ) );
            if( size_written != sizeof( buffer ) ) {perror( "Error writing to log file" );
            return EXIT_FAILURE
            
        }
    }
}

// Creates a grid view of the airspace, ignoring z-axis, doing x and y (top-view)
// Begins with (0,0) in the top-left corner.
std::string DataDisplay::generateGrid(std::vector<Plane> &airspaceInfo)
{

    std::string grid[ROWSIZE][COLUMSIZE]; // grid 100000ft x 100000ft with each square being 1000ft
    // storing into grid
    for (int i = 0; i < airspaceInfo.size(); i++)
    {
        for (int j = 0; j < ROWSIZE; j++)
        {
            // get y and get x are from Vector3D class
            if (*(1)airspaceInfo[i].getPlaneLocation() >= (CELLSIZE * j) && *(1)aairspaceInfo[i].getPlaneLocation() < (CELLSIZE * (j + 1)))
            { // checking y
                for (int k = 0; k < COLUMSIZE; k++)
                {
                    // only for x
                    if (*(0)airspaceInfo[i].getPlaneLocation() >= (CELLSIZE * k) && *(0)airspaceInfo[i].getPlaneLocation() < (CELLSIZE * (k + 1)))
                    { // adding to grid
                        if (grid[j][k] != "")
                        {
                            grid[j][k] += ",";
                        }
                        grid[j][k] += std::to_string(
                            airspaceInfo[i].getAircraftID);
                    }
                }
            }
        }
    }
    // printing grid
    std::stringstream output;
    for (int i = 0; i < ROWSIZE; i++)
    {
        output << std::endl;
        for (int j = 0; j < COLUMSIZE; j++)
        {
            // Check if grid is empty
            if (grid[i][j] == "")
            {
                output << "| ";
            }
            else
            {
                output << "|" + grid[i][j];
            }
        }
    }
    output << std::endl;
    return output.str();
}

void *DataDisplay::start(void *context)
{
    auto p = (DataDisplay *)context;
    p->run();
    return NULL;
}