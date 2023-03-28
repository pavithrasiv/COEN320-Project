#ifndef DATADISPLAY_H
#define DATADISPLAY_H
#include "Plane.h"
#include <string>
#include <vector>

class DataDisplay
{
private:
    void run();
    void receiveMessage();
    std::string createGrid(std::vector<Plane>);

    int channelID;
    std::vector<Plane> planes;
    int fd;


public:
    DataDisplay();
    int getChannelID() const;
    static void start(void *context);
};
#endif