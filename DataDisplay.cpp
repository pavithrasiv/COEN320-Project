#include "DataDisplay.h"
#include <sys/neutrino.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sstream>

#define ROWSIZE 100
#define COLUMSIZE 100
#define CELLSIZE 25

#define SINGLE_PLANE 1
#define MULTIPLE_PLANE 2
#define GRID 3
#define LOG 4
DataDisplay::DataDisplay() : channelID(-1), logError(-1)
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
        std::cout << "Creation of channel failed. Exiting thread." << std::endl;
        return;
    }

    // Open log file
    logError = creat("/data/home/qnxuser/log.txt", S_IRUSR | S_IWUSR | S_IXUSR);
    if (logError == -1)
    {
        std::cout << "DataDisplay failed to open logfile. Errno is "
                  << errno << std::endl;
    }

    receiveMessage(); // start to listen for messages

    // Close log file
    if (logError != -1)
    {
        close(logError);
    }
}

void DataDisplay::receiveMessage()
{
    int receiveMessageID;   // receive id
    dataDisplayMessage msg; //
    while (1)
    {

        receiveMessageID = MsgReceive(channelID, &msg, sizeof(msg), NULL); // get id for that specific message

        switch (msg.command)
        {
        case SINGLE_PLANE:
        {
            // Command to display one plane
            MsgReply(receiveMessageID, EOK, NULL, 0); // sending basic ACK
            std::cout << "Aircraft ID: " << msg.aircraft.single.aircraftID
                      << std::endl
                      << "Aircraft position: "
                      << msg.aircraft.single.position
                      << std::endl
                      << "Aircraft velocity " << msg.aircraft.single.velocity
                      << std::endl;
            break;
        }
        case MULTIPLE_PLANE:
        {
            // Command to display multiple planes
            MsgReply(receiveMessageID, EOK, NULL, 0);
            for (int i = 0; i < msg.aircraft.multiple.numberOfAircrafts; // get total aircrafts
                 i++)
            {
                // std::cout <<"Aircraft positions: " <<msg.CommandBody.multiple->positionArray <<"  " <<"Aircraft velocities: " <<msg.commandBody.multiple->velocityArray <<std::endl;
                std::cout << "Aircraft " << i + 1 << " with position: "
                          << msg.aircraft.multiple.positionArray[i]
                          << " and velocity: "
                          << msg.aircraft.multiple.velocityArray[i]
                          << std::endl;
            }
            break;
        }
        case GRID: // Top View grid (X and Y)
        {
            // Command to print a grid to the console
            // The sender deletes the arrays allocated for grid printing once we reply, so we need them to stay valid until then.
            std::cout << generateGrid(msg.aircraft.multiple);
            MsgReply(receiveMessageID, EOK, NULL, 0);
            break;
        }
        case LOG:
        {
            // Command to print a grid to the log file
            std::string grid = generateGrid(msg.aircraft.multiple);
            MsgReply(rcvid, EOK, NULL, 0);
            if (logError != -1)
            {
                // TODO:print Logs
            }
            else
            {
                std::cout
                    << "DataDisplay: Received a log command but the log file is not opened."
                    << std::endl;
            }
            break;
        }
        case EXIT_THREAD:
            MsgReply(receiveMessageID, EOK, NULL, 0);
            return;
        }
    }
}

// Creates a grid view of the airspace, ignoring z-axis, doing x and y (top-view)
// Begins with (0,0) in the top-left corner.
std::string DataDisplay::generateGrid(multipleAircraftDisplay &airspaceInfo)
{

    std::string grid[ROWSIZE][COLUMSIZE]; // grid 100000ft x 100000ft with each square being 1000ft
    // storing into grid
    for (int i = 0; i < airspaceInfo.numberOfAircrafts; i++)
    {
        for (int j = 0; j < ROWSIZE; j++)
        {
            // get y and get x are from Vector3D class
            if (airspaceInfo.positionArray[i].getypostion >= (CELLSIZE * j) && airspaceInfo.positionArray[i].getypostion < (CELLSIZE * (j + 1)))
            { // checking y
                for (int k = 0; k < COLUMSIZE; k++)
                {
                    // only for x
                    if (airspaceInfo.positionArray[i].getxpostion >= (CELLSIZE * k) && airspaceInfo.positionArray[i].getxpostion < (CELLSIZE * (k + 1)))
                    { // adding to grid
                        if (grid[j][k] != "")
                        {
                            grid[j][k] += ",";
                        }
                        grid[j][k] += std::to_string(
                            airspaceInfo.planeIDArray[i]);
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