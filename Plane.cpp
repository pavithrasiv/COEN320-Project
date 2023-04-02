#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
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


//The Server
//Create the channel
//Receive the available messages
//Check for any error message
//Reply to client either using MsgReply(…) or msgError(…)
//Print the content of the messages (velocity and position)
//Destroy the channel

void* server(void*) {
	//Name of attach value for the channel and connection
	name_attach_t *name_attach_t;

	//Instantiate message based on structure
	my_data_t msg;

	//Create receive ID
	int rcvid;

	//To create a channel, and attach it to server : name_attach_t =
	//name_attach(dispatch_t * dpp, const char * path, unsigned flags);
	//Path is the name of the channel
	if ((name_attach_t = name_attach(NULL, ATTACH_POINT, 0)) == NULL){
		perror("Error occurred during creation of channel");
	}

	//To receive a message from the client
	//int MsgReceive(int chid, void * msg, size_t size, struct _msg_info * info);
	//chid is the channel ID of the server
	//*msg is the pointer to the variable where the message will be stored
	//size is the size in bytes of msg
	//*info is a NULL, or a pointer to a msg_info structure
	//Do MsgReceive here with the chid in a while(true) loop
	while(true){
		//Server will block until it receives message from client through
		//a channel named "name_attach_t" which was created with name_attach(..)
		//The received message will be stored in msg and rcvid
		rcvid = MsgReceive(name_attach_t->chid, &msg, sizeof(msg), NULL);

		//In case of error
		if(rcvid == -1) {
			perror("Error receiving message");
			break;
		}

		//Else, if properly received message/pulse
		//Pulse signal actions depending on code
		if (rcvid == 0){
			//Note : hdr comes from hdr in msg struct
			switch (msg.hdr.code){
			//Client disconnected connections
			case _PULSE_CODE_DISCONNECT:
				ConnectDetach(msg.hdr.scoid);
				break;
			//Reply blocked clients wnat to unblock
			case _PULSE_CODE_UNBLOCK:
				break;
			default:
				break;
		}
		continue;
		}


		//name_open() sends a connect message, must EOK this */
		if (msg.hdr.type == _IO_CONNECT ) {
			MsgReply( rcvid, EOK, NULL, 0 );
			continue;
		}



	//Reply to client error case
	//Use this if message received not in expected format
	//int MsgError(int rcvid, int error)
	//rcvid is the output of receive function
	//error can be set to -1, ENOSYS, ERESTART, EOK, or the error code that you want to set for the client
	//First check if msg is what we expect
	//Then you can have a switch that checks subtype of message (like 0x01 for speed, 0x02 for temperature, etc)
	//Build GUI output based on the msg subtypes
	//0x00 will be for data
	if (msg.hdr.type == 0x00){
		//Motion message received
		//0x01 is subtype for velocity
		if (msg.hdr.subtype == 0x01){
			printf("The speed of the plane is  %.2f m/s \n", msg.data);
		}
		//0x02 is for position
		else if (msg.hdr.subtype == 0x02){
			printf("The x position of the plane is %.2f m \n", msg.data);
			printf("\n");
		}
		else{
			MsgError(rcvid, ENOSYS);
			continue;
		}
	}

	MsgReply(rcvid, EOK, 0, 0);
	}


	//Finally, to destroy a channel
	//Do int name_detach(name_attach_t * attach, unsigned flags);
	//attach is a pointer to the attach object received as output of the name_attach(…) function
	//flags are parameters that affect function’s behavior (just use 0)
	name_detach(name_attach_t, 0);

	return NULL;
}


void* client(void *){
	my_data_t speed_msg;
	my_data_t position_msg;

	//Server connection ID
	int server_coid;

	//First, the client must open a channel to connect to server
	//use int name_open(const char * name, int flags);
	//where name is the name of the channel created by the server
	//flags are parameters that affect the function’s behavior (use 0)
	if ((server_coid = name_open(ATTACH_POINT, 0)) == -1){
		perror("Error occurred while attaching the channel");
	}

	double speed=0;
	double position=0;

	uint32_t period_sec=2;

	double period=period_sec;

	Timer timer(period_sec,period_sec );

	double acceleration=0.1;
	int count=0;
	double t;

	while(t<30.0){
		t=count*period;
		printf("Iteration number %d: t=%.2f sec \n",count+1,t);

		speed+=acceleration*t;
		position+=speed;

		//Define types and subtypes
		speed_msg.hdr.type=0x00;
		speed_msg.hdr.subtype=0x01;
		speed_msg.data=speed;

		printf("Client sending speed %.2f sec \n", speed_msg.data);

		//Next step is to send the message to server
		//long MsgSend(int coid, const void* smsg, size_t ssize, void* rmsg, size_t rsize);
		//coid is the connection id return by name_open(…) function
		//smsg pointer to variable containing message you want to send to server
		//ssize is the size of the msg
		//rmsg is optional, is variable to reply from server
		//rsize is the size of reply of server
		if (MsgSend(server_coid, &speed_msg, sizeof(speed_msg), NULL,0)==-1){
			perror("Error sending speed message");
			break;
		}


		position_msg.hdr.type=0x00;
		position_msg.hdr.subtype=0x02;
		position_msg.data=position;
		if (MsgSend(server_coid, &position_msg, sizeof(position_msg), NULL, 0) == -1){
			perror("Error sending position message");
			break;
		}

		timer.wait_next_activation();
		count++;


	}

	//Finally, close connection with server when done
	//int name_close(int coid);
	//coid is the connection ID returned by name_open() function
	name_close(server_coid);
	return EXIT_SUCCESS;
}


PlaneClass::PlaneClass(int id, int posX, int posY, int posZ, int velX, int velY, int velZ, int time)
{
    outsideAirspace = false;
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
}

PlaneClass::~PlaneClass() {

}

int main(int argc, char*argv[]){
	char cwd[256];
	    if (getcwd(cwd, sizeof(cwd)) != NULL) {
	        std::cout << "Current working directory is: " << cwd << std::endl;
	    } else {
	        std::perror("getcwd() error");
	    }
	vector<PlaneClass> planes = readPlanesFromFile("./planes.txt");
	for (const auto& plane : planes) {
	    cout << "ID: " << plane.getAircraftID() << endl;
	    cout << "Arrival Time: " << plane.getArrivalTime() << endl;
	    cout << "Position: (" << plane.getPosition(0) << ", " << plane.getPosition(1) << ", " << plane.getPosition(2) << ")" << endl;
	    cout << "Velocity: (" << plane.getVelocity(0) << ", " << plane.getVelocity(1) << ", " << plane.getVelocity(2) << ")" << endl;
	}
	//Declare thread
	pthread_t server_thread;
	int err_no;
	err_no = pthread_create(&server_thread, NULL, &server, NULL);
	if (err_no != 0){
		perror("Error creating server thread! \n");
	}

	pthread_t client_thread;
	err_no = pthread_create(&client_thread, NULL, &client, NULL);
	if (err_no != 0){
		perror("Error creating client thread! \n");
	}

	//Join thread to start it
	err_no = pthread_join(server_thread, NULL);
	if (err_no != 0){
		perror("Error joining server thread!");
	}

	err_no = pthread_join(client_thread, NULL);
	if (err_no != 0){
		perror("Error joining client thread!");
	}
	pthread_exit(EXIT_SUCCESS);
	return 0;
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
            cout << "Position: " << *pos[0] << endl;
            cout << "Position: " << *pos[1] << endl;
            cout << "Position: " << *pos[2] << endl;
            int posX = *pos[0];
            int posY = *pos[1];
            int posZ = *pos[2];
            int *vel[3] = {xVelArr, yVelArr, zVelArr};
            cout << "Velocity: " << *vel[0] << endl;
            cout << "Velocity: " << *vel[1] << endl;
            cout << "Velocity: " << *vel[2] << endl;
            int velX = *vel[0];
            int velY = *vel[1];
            int velZ = *vel[2];
            planes.emplace_back(id, posX, posY, posZ, velX, velY, velZ, arrivalTime);
        }
    }
    return planes;
}






