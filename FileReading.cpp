#include <iostream>
#include "FileReading.h"
#include <fstream>
#include <string>

using namespace std;

void readingFile(string fileName){
    ifstream inputFile;
    inputFile.open(fileName);

    //check if the file has been opened or not 
    if(!inputFile){
        cout << "not able to open the file" << endl;
        return;
    }

    //will have to look at the format of the inputs 
    //and store them into variables that could be used in other files 
    //(commsys, compsys, airplane...)

    string readLine;
    while(getline(inputFile, readLine)){
        cout << readLine << endl;
    }

    inputFile.close();
}
