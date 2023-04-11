#ifndef DATADISPLAY_H
#define DATADISPLAY_H
#include "PlaneClass.h"
#include <string>
#include <vector>

class DataDisplay
{
    typedef struct planeClass
    {
        std::vector<PlaneClass> planes;
        PlaneClass singlePlane;
        int command;
    } planeInfo;

private:
        void receiveMessage();
    std::string createGrid(std::vector<PlaneClass>);
    int channelID;
    int fd;

public:
    DataDisplay();
    int getChannelID() const;
    void run();
};
#endif