#include "Position.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>

typedef struct _pulse msg_header_t;

typedef enum {OperatorSetAirplaneSpeed, OperatorRequestAirplaneData} Msg2CommSysType;
typedef enum {CommSysSetAirplaneSpeed, CommSysRequestAirplaneData, ClockTimerUpdate, AirplaneScan } AirplaneMsgType;

typedef struct  {
	msg_header_t hdr;
	Msg2CommSysType type;
	int id;
	float x, y, z;
	float dx, dy, dz;
} Msg2CommSys;

typedef struct  {
	msg_header_t hdr;
	AirplaneMsgType type;
	int id;
	float x, y, z;
	float dx, dy, dz;
} Msg2Airplane;
