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
#define GRID 2
#define LOG 3
DataDisplay::DataDisplay() : channelID(-1) fd(-1)
{
}

int DataDisplay::getChannelID() const
{
    return channelID;
}

void DataDisplay::run()
{
    // get Channel for command
    int commandChannelID = channelCreate(0);
    int planeChannelID = channelCreate(1);

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
    int planeMessageID;
    int vectorPlanesID; //
    while (1)
    {
        planeMessageID = MsgReceive(commandChannelID, &msg, sizeof(msg), NULL); // get id for warning 1 plane
        vectorPlanesID = MsgReceive(planeChannelID, planeInfo, sizeof(planeInfo), NULL);

        switch (planeInfo.command)
        {
        case WARNING:
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
            std::cout << generateGrid(planeInfo.planes);
            MsgReply(vectorPlanesID, EOK, NULL, 0);
            break;
        }
        case LOG:
        {
            // Command to print a grid to the log file
            char buff[] = generateGrid(planeInfo.planes);
            MsgReply(vectorPlanesID, EOK, NULL, 0);
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