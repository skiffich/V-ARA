#include "ofApp.h"

// 12/12/17
// Receive string from Arduino and convert string to float values
// Arduino connected to COM5 with baud rate 57600
// Arduino sends string using Firmata protocol when
// it receives "N" string using Firmata protocol
// Because when arduino will send string continuously,
// Program will stay in "String received handler" function
//
// Load to arduino sketch Arduino.ino from Arduino folder
/*
* Arduino-Tensoresistors-IMU-Unity3D uniting sketch
*
* Change the string to your values in TODO
* Note, you can not change a size of string or position of your value
* just paste it into a place acording to template
* Tr1-5 values must be in range from 00 to 99
* X-Zacc values must be in range from 000 to -99/099 and first mark must be or '0' or '-' if value is less then 0
* X-Zgyr values must be in range from 0000 to -180/0180 and first mark must be or '0' or '-' if value is less then 0
*
* A - Xacc - 3ch
* B - Yacc - 3ch
* C - Zacc - 3ch
* D - Xgyr - 4ch
* E - Ygyr - 4ch
* F - Zgyr - 4ch
* G - Tr1  - 2ch
* H - Tr2  - 2ch
* I - Tr3  - 2ch
* J - Tr4  - 2ch
* K - END  - 0ch
*
* 40 bytes in total
*
* "A000B000C000D0000E0000F0000G00H00I00J00K"     // template
* " 1   5   9   13   18   23   28 31 34 37 "     // positions
* "A099B001C-15D0179E-179F0025G01H99I38J65K"     // example
*/

//--------------------------------------------------------------
void ofApp::setup(){
	// Connect to arduino on COM5 with baud rate 57600
	ard.connect("COM5", 57600);
	// String received handler connection
	ofAddListener(ard.EStringReceived, this, &ofApp::stringReceived);
}

//--------------------------------------------------------------
// String received handler
void ofApp::stringReceived(const string & string) {
	// save received string
	received = string;
}

//--------------------------------------------------------------
void ofApp::update(){
	try {
		// Convert received string to floats
		// Convert accelerometer values
		std::string sAccX = received.substr(1, 3);
		stringstream sAccXstream(sAccX);
		sAccXstream >> xAcc;
		std::string sAccY = received.substr(5, 3);
		stringstream sAccYstream(sAccY);
		sAccYstream >> yAcc;
		std::string sAccZ = received.substr(9, 3);
		stringstream sAccZstream(sAccZ);
		sAccZstream >> zAcc;
		// Convert gyroscope values
		std::string sGyrX = received.substr(13, 4);
		stringstream sGyrXstream(sGyrX);
		sGyrXstream >> xGyr;
		std::string sGyrY = received.substr(18, 4);
		stringstream sGyrYstream(sGyrY);
		sGyrYstream >> yGyr;
		std::string sGyrZ = received.substr(23, 4);
		stringstream sGyrZstream(sGyrZ);
		sGyrZstream >> zGyr;
		// Convert tensoresistors values
		std::string sTen1 = received.substr(28, 2);
		stringstream sTen1stream(sTen1);
		sTen1stream >> ten1;
		std::string sTen2 = received.substr(31, 2);
		stringstream sTen2stream(sTen2);
		sTen2stream >> ten2;
		std::string sTen3 = received.substr(34, 2);
		stringstream sTen3stream(sTen3);
		sTen3stream >> ten3;
		std::string sTen4 = received.substr(37, 2);
		stringstream sTen4stream(sTen4);
		sTen4stream >> ten4;
		// Show converted values
		cout << xAcc << " " << yAcc << " " << zAcc << " " << xGyr << " " <<
				yGyr << " " << zGyr << " " << ten1 << " " << ten2 << " " <<
				ten3 << " " << ten4 << endl;

		// Send "N" to arduino
		ard.sendString("N");
		// Apdate arduino
		ard.update();
	}
	catch (...)
	{
		// TO DO OR NOT TO DO
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
