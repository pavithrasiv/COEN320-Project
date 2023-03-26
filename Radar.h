#ifndef RADAR_H
#define RADAR_H

#define AIRSPACEHEIGHT 2500
#define AIRSPACELENGTH 10000
#define AIRSPACEWIDTH 10000

#include <vector>
#include <array>
#include <map>

class Radar
{
public:
    Radar();

    Radar(std::vector<Plane> &planes);
    std::vector<int *> scanAirspace(std::vector<Plane> &planes);
    void sendRadarScan();

private:
    std::vector<Plane> planes;
    int airspace[AIRSPACELENGTH][AIRSPACEWIDTH][AIRSPACEHEIGHT];
};

#endif