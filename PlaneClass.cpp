#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#define MIN_AIRSPACE_X_REGION 0
#define MAX_AIRSPACE_X_REGION 100000
#define MIN_AIRSPACE_Y_REGION 0
#define MAX_AIRSPACE_Y_REGION 100000
#define MIN_AIRSPACE_Z_REGION 15000
#define MAX_AIRSPACE_Z_REGION 25000
#include "PlaneClass.h"



//Thread that receives message (server) needs a channel, and thread that sends message
//Must create a connection that attaches to the channel
#define ATTACH_POINT "my_channel"

//Message header is at least a pulse
//This identifies the message that is being sent and received
//Unique ID for it basically
typedef struct _pulse msg_header_t;

//Format of the data
typedef struct _my_data {
	msg_header_t hdr;
	double data;
} my_data_t;

//Format of plane
typedef struct _plane_data {
	msg_header_t hdr;
	int id;
	int velocity[3];
	int position[3];
    bool outsideAirspace;
    bool ArrivedYet;
} plane_data_t;


void* PlaneClientThread(void *)
{
	//plane_data_t velocity_msg;
	//plane_data_t position_msg;
	plane_data_t airspace_msg;

	//Server connection ID
	int radarserver_coid;

	//First, the client must open a channel to connect to server
	//use int name_open(const char * name, int flags);
	//where name is the name of the channel created by the server
	//flags are parameters that affect the function’s behavior (use 0)
	if ((radarserver_coid = name_open(ATTACH_POINT, 0)) == -1) {
			perror("Error occurred while attaching the channel");
		}

	vector<PlaneClass> planes = readPlanesFromFile("./planes.txt");
	uint32_t period_sec=1;
	double period=period_sec;
	Timer timer(period_sec,period_sec);
	int count=0;
	double t = 0;
	airspace_msg.hdr.type=0x00;
	airspace_msg.hdr.subtype=0x01;
	while(t<30.0){
		t=count*period;
					for (PlaneClass& plane : planes) {
					if (plane.getArrivalTime() > t){

					    airspace_msg.id = plane.getAircraftID();
						for (int i = 0; i < 3; i++) {
						    airspace_msg.velocity[i] = plane.getVelocity(i);
						    airspace_msg.position[i] = plane.getPosition(i);
						}
						if(plane.outsideAirspace()){
							airspace_msg.outsideAirspace = true;
						}
						else {
							airspace_msg.outsideAirspace = false;
						}
						airspace_msg.ArrivedYet = false;
					    if (MsgSend(radarserver_coid, &airspace_msg, sizeof(airspace_msg), NULL,0) == -1){
					    	perror("Error sending speed message");
					    	break;
					    		}

					}
					else{
					    plane.update();
					    //cout << "Position: (" << plane.getPosition(0) << ", " << plane.getPosition(1) << ", " << plane.getPosition(2) << ")" << endl;
					    //cout << "Velocity: (" << plane.getVelocity(0) << ", " << plane.getVelocity(1) << ", " << plane.getVelocity(2) << ")" << endl;
						//Define types and subtypes
					    airspace_msg.id = plane.getAircraftID();
					    airspace_msg.ArrivedYet = true;
						for (int i = 0; i < 3; i++) {
						    airspace_msg.velocity[i] = plane.getVelocity(i);
						    airspace_msg.position[i] = plane.getPosition(i);
						}
						if(plane.outsideAirspace()){
							airspace_msg.outsideAirspace = true;
						}
						else {
							airspace_msg.outsideAirspace = false;
						}
						//Define types and subtypes
						//airspace_msg.hdr.type=0x00;
						//airspace_msg.hdr.subtype=0x02;
						//airspace_msg.position=positionTrue;
						//airspace_msg.hdr.type=0x00;
						//airspace_msg.hdr.subtype=0x03;
						//airspace_msg.airspace=positionTrue;
					    if (MsgSend(radarserver_coid, &airspace_msg, sizeof(airspace_msg), NULL,0) == -1){
					    	perror("Error sending speed message");
					    	break;
					    		}
					}
					}
					cout << "Time is " << t << endl;
					timer.wait_next_activation();
					count++;

	}

	//Finally, close connection with server when done
	//int name_close(int coid);
	//coid is the connection ID returned by name_open() function
	name_close(radarserver_coid);
	return EXIT_SUCCESS;
}





void PlaneClass::update(){
    positionTrue[0] += velocity[0];
    positionTrue[1] += velocity[1];
    positionTrue[2] += velocity[2];

}



PlaneClass::PlaneClass(int id, int posX, int posY, int posZ, int velX, int velY, int velZ, int time)
{
    outsideAirspaceStatus = true;
    positionTrue[0] = posX; // assign X value
    positionTrue[1] = posY; // assign Y value
    positionTrue[2] = posZ; // assign Z value
    velocity[0] = velX;
    velocity[1] = velY;
    velocity[2] = velZ;
    aircraftID = id;
    arrivalTime = time;
}

int PlaneClass::getAircraftID() const {
    return aircraftID;
}

int PlaneClass::getArrivalTime() const {
    return arrivalTime;
}

int PlaneClass::getPosition(int posValue) const {
    if(posValue==0){
	return positionTrue[0];
    }
    else if (posValue==1){
    	return positionTrue[1];
    }
    else if (posValue==2){
    	return positionTrue[2];
    }
    else return 0;
}

int PlaneClass::getVelocity(int posValue) const {
    if(posValue==0){
	return velocity[0];
    }
    else if (posValue==1){
    	return velocity[1];
    }
    else if (posValue==2){
    	return velocity[2];
    }
    else return 0;
}

bool PlaneClass::outsideAirspace(){

	    if (positionTrue[0] < MIN_AIRSPACE_X_REGION || positionTrue[0] > MAX_AIRSPACE_X_REGION || positionTrue[1] < MIN_AIRSPACE_Y_REGION || positionTrue[1] > MAX_AIRSPACE_Y_REGION || positionTrue[2] < MIN_AIRSPACE_Z_REGION || positionTrue[2] > MAX_AIRSPACE_Z_REGION)
	    {
	        outsideAirspaceStatus = true;
	    }

	    else {
	    	outsideAirspaceStatus = false;
	    }
	    return outsideAirspaceStatus;
}

PlaneClass::~PlaneClass() {
    for(int i=0;i<3;i++) {
        positionTrue[i] = 0;
        velocity[i] = 0;
    }

}

vector<PlaneClass> readPlanesFromFile(string fileName) {
    vector<PlaneClass> planes;
    ifstream inputFile(fileName);
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        int id, arrivalTime;
        double xPos, yPos, zPos, xSpeed, ySpeed, zSpeed;
        char comma;
        if (ss >> arrivalTime >> comma >> id >> comma >> xPos >> comma >> yPos >> comma >> zPos >> comma >> xSpeed >> comma >> ySpeed >> comma >> zSpeed) {
            cout << "id: " << id << endl;
            cout << "arrivalTime: " << arrivalTime << endl;
            cout << "xPos: " << xPos << endl;
            cout << "yPos: " << yPos << endl;
            cout << "zPos: " << zPos << endl;
            cout << "xSpeed: " << xSpeed << endl;
            cout << "ySpeed: " << ySpeed << endl;
            cout << "zSpeed: " << zSpeed << endl;
            int xPosArr[] = { static_cast<int>(xPos) };
            int yPosArr[] = { static_cast<int>(yPos) };
            int zPosArr[] = { static_cast<int>(zPos) };
            int xVelArr[] = { static_cast<int>(xSpeed) };
            int yVelArr[] = { static_cast<int>(ySpeed) };
            int zVelArr[] = { static_cast<int>(zSpeed) };
            int *pos[3] = {xPosArr, yPosArr, zPosArr};
            int posX = *pos[0];
            int posY = *pos[1];
            int posZ = *pos[2];
            int *vel[3] = {xVelArr, yVelArr, zVelArr};
            int velX = *vel[0];
            int velY = *vel[1];
            int velZ = *vel[2];
            planes.emplace_back(id, posX, posY, posZ, velX, velY, velZ, arrivalTime);
        }
    }
    return planes;
}










