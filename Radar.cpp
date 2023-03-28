#include "Plane.h"
#include "Radar.h"

Radar::Radar()
{
}

std::vector<int *> Radar::scanAirspace(std::vector<Plane> &planes)
{
    std::vector<int *> planePositions;
    for (int i = 0; i < planes.size(); i++)
    {
        planePositions.push_back(planes.getPlaneLocation());
    }
    return planePositions;
}

void Radar::sendRadarScan()
{

    // Declare channel
    // int coid = ConnectAttach(0, 0, p.getChid(), _NTO_SIDE_CHANNEL, 0);
    // MsgSend(coid, &msg, sizeof(msg), &res, sizeof(res));
}
