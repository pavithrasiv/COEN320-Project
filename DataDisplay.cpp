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
#define ROWS 50
#define COLUMS 50
#define CELLSIZE 25

#define WARNING 1
#define PLANE 2
#define GRID 3
#define LOG 4
DataDisplay::DataDisplay() : channelID(-1) fd(-1)
{
}

void DataDisplay::begin()
{
    // get Channel for command
    channelID = channelCreate(0);

    // TODO: get Channel from Plane
    if (channelID == -1)
    {
        std::cout << "Channel failed to create. Exiting thread." << std::endl;
        return;
    }

    fd = creat("/data/home/qnxuser/myfile.dat", S_IRUSR | S_IWUSR | S_IXUSR);
    if (fd == -1)
    {
        cout << "Log file could not be created" << endl;
    }
    receiveMessage(); // start to listen for messages
}

void DataDisplay::receiveMessage()
{
    int receiveMessageID; // receive id
    planeInfo msg;
    while (1)
    {
        receiveMessageID = MsgReceive(channelID, msg, sizeof(msg), NULL);

        switch (msg.command)
        {
        case WARNING:
        {
            // Command to display Warning of one plane
            MsgReply(receiveMessageID, EOK, NULL, 0); // sending ACK
            std::cout << "WARNING: Aircraft ID: " << msg.singlePlane.getAircraftID()
                      << std::endl
                      << "Aircraft position x: " << msg.singlePlane.getPosition(0) << "y: " << msg.singlePlane.getPosition(1) << "z: " << msg.singlePlane.getPosition(2)
                      << std::endl
                      << "Aircraft velocity x: " << msg.singlePlane.getVelocity(0) << "y: " << msg.singlePlane.getVelocity(1) << "z: " << msg.singlePlane.getVelocity(2)
                      << " is on a TRAJECTORY TO CRASH!"
                      << std::endl;
            break;
        }
        case PLANE:
        {
            MsgReply(receiveMessageID, EOK, NULL, 0); // sending ACK
            std::cout << "Aircraft ID: " << msg.singlePlane.getAircraftID()
                      << std::endl
                      << "Aircraft position x: " << msg.singlePlane.getPosition(0) << "y: " << msg.singlePlane.getPosition(1) << "z: " << msg.singlePlane.getPosition(2)
                      << std::endl
                      << "Aircraft velocity x: " << msg.singlePlane.getVelocity(0) << "y: " << msg.singlePlane.getVelocity(1) << "z: " << msg.singlePlane.getVelocity(2)
                      << std::endl;
            break;
        }
        case GRID: // Top View grid (X and Y)

        {
            std::cout << generateGrid(planeInfo.planes);
            MsgReply(vectorPlanesID, EOK, NULL, 0);
            break;
        }
        case LOG:
        {
            // Command to print a grid to the log file
            char buff[] = generateGrid(planeInfo.planes);
            MsgReply(receiveMessageID, EOK, NULL, 0);
            size_written = write(fd, buff, sizeof(buff));
            if (size_written != sizeof(buff))
            {
                perror("Error writing to log file");
                return EXIT_FAILURE
            }
        }
        }
    }
}
// Creates a grid view of the airspace, ignoring z-axis, doing x and y (top-view)
// Begins with (0,0) in the top-left corner.
std::string DataDisplay::generateGrid(std::vector<PlaneClass> &airspaceInfo)
{

    std::string grid[ROWS][COLUMS];
    for (int i = 0; i < airspaceInfo.size(); i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            // get y and get x are from Vector3D class
            if (airspaceInfo[i].getPosition(1) >= (CELLSIZE * j) && airspaceInfo[i].getPosition(1) < (CELLSIZE * (j + 1)))
            { // checking y
                for (int k = 0; k < COLUMS; k++)
                {
                    // only for x
                    if (airspaceInfo[i].getPosition(0) >= (CELLSIZE * k) && airspaceInfo[i].getPosition(0) < (CELLSIZE * (k + 1)))
                    { // adding to grid
                        if (grid[j][k] != "")
                        {
                            grid[j][k] += " & ";
                        }
                        grid[j][k] += std::to_string(airspaceInfo[i].getAircraftID());
                    }
                }
            }
        }
    }
    // printing grid
    std::stringstream output;
    for (int i = 0; i < ROWS; i++)
    {

        for (int j = 0; j < COLUMS; j++)
        {
            // Check if grid is empty
            if (grid[i][j] == "")
            {
                output << "|        ";
            }
            else
            {
                output << "|";
                output << grid[i][j] << "    ";
            }
        }
        output << std::endl;
        for (int i = 0; i < 10 * ROWS; i++)
        {
            output << "-";
        }
        output << std::endl;
    }
    output << std::endl;

    return output.str();
}