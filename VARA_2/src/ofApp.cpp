#include "ofApp.h"

// 13/12/17
// Receive string from Arduino and convert string to float values
// Arduino connected to any COM port with baud rate 57600
// Arduino sends string using Firmata protocol when
// it receives "N" string using Firmata protocol
// Because when arduino will send string continuously,
// Program will stay in "String received handler" function
//
// Load to arduino sketch Arduino.ino from Arduino folder
/*
*
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
/* Errors & Exceptions:
* 1 (Solved)
* Strange Exception in runtime caused by unknown what by and unknown where
* Exception: "vector iterator not dereferencable"
* Probably the reason is the non-sync threads in  "stringReceived" and "update"
* Result: I think it is fixed with adding delay between transactions to Arduino
*/

//--------------------------------------------------------------
void ofApp::setup(){
	// Set background to black
	ofBackground(0);
	// Connect to arduino connected to any COM port with baud rate 57600
	try
	{
		ofSerial serial;
		vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
		for each (ofSerialDeviceInfo var in deviceList)
		{
			ofLogNotice() << "Connecting to " << var.getDeviceName();
			ard.connect(var.getDeviceName(), 57600);
			if (ard.isArduinoReady())
			{
				ofAddListener(ard.EStringReceived, this, &ofApp::stringReceived);
				ofLogNotice() << "Connected to Arduino on " << var.getDeviceName();
				ofLogNotice() << "Setup...";
				ofLogNotice() << ard.getFirmwareName();
				ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
				bSetupArduino = true;
				wasConnected = true;
				break;
			}
		}
	}
	catch (...)
	{
		ofLogError() << "Connecting failed";
	}
	// String received handler connection
	ofAddListener(ard.EStringReceived, this, &ofApp::stringReceived);
	// Setup clock_start
	clock_start = clock();
}

//--------------------------------------------------------------
// String received handler
void ofApp::stringReceived(const string & string) {
	// save received string
	received = string;
	// Apdate arduino
	ard.update();
}

// Convert received string to received data
void ofApp::convertReceivedString2ReceivedData(const string string)
{
	//cout << string << endl;
	// Convert accelerometer values
	std::string sAccX = received.substr(1, 3);
	stringstream sAccXstream(sAccX);
	sAccXstream >> xAcc;
	xAcc *= 5;
	std::string sAccY = received.substr(5, 3);
	stringstream sAccYstream(sAccY);
	sAccYstream >> yAcc;
	yAcc *= 5;
	std::string sAccZ = received.substr(9, 3);
	stringstream sAccZstream(sAccZ);
	sAccZstream >> zAcc;
	zAcc *= 5;
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
	//* Show converted values
	cout << xAcc << " " << yAcc << " " << zAcc << " " << xGyr << " " <<
		yGyr << " " << zGyr << " " << ten1 << " " << ten2 << " " <<
		ten3 << " " << ten4 << endl; //*/
}

//--------------------------------------------------------------
void ofApp::update(){
	// Calculate current clocks interval
	double cur_clock = double(clock() - clock_start) / CLOCKS_PER_SEC;
	// Update arduino if duration has been more than 0.05 seconds
	if (cur_clock > 0.05 && bSetupArduino) {
		// reset clock_start
		clock_start = clock();
		try {
			// Send "N" to arduino
			ard.sendString("N");
			// Apdate arduino
			ard.update();
		}
		catch (...)
		{
			ofLogError() << "Connecting failed";
			bSetupArduino = false;
		}
		// Convert received string to received data
		convertReceivedString2ReceivedData(received);
	}
	else if (cur_clock > 0.05 && !bSetupArduino && wasConnected)
	{
		try
		{
			ofSerial serial;
			vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
			for each (ofSerialDeviceInfo var in deviceList)
			{
				ofLogNotice() << "Connecting to " << var.getDeviceName();
				ard.connect(var.getDeviceName(), 57600);
				if (ard.isArduinoReady())
				{
					ofAddListener(ard.EStringReceived, this, &ofApp::stringReceived);
					ofLogNotice() << "Connected to Arduino on " << var.getDeviceName();
					ofLogNotice() << "Setup...";
					ofLogNotice() << ard.getFirmwareName();
					ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
					bSetupArduino = true;
					break;
				}
			}
		}
		catch (...)
		{
			ofLogError() << "Connecting failed";
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();

	// Set a start point in the center of the screen
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 40);

	
	// Rotate to the angle specified by the user
	ofVec3f axis;
	float angle;
	curRot.getRotate(angle, axis);
	ofRotate(angle, axis.x, axis.y, axis.z);

	//ofPushMatrix();

	// Rotate to the angle obtained from the gyroscope
	ofRotateX(xGyr);
	ofRotateY(yGyr);
	ofRotateZ(zGyr);

	// Draw coordinate system
	ofSetColor(ofColor::red);
	ofDrawLine(0, 0, 0, 100, 0, 0);
	ofSetColor(ofColor::blue);
	ofDrawLine(0, 0, 0, 0, 100, 0);
	ofSetColor(ofColor::yellow);
	ofDrawLine(0, 0, 0, 0, 0, 100);
	// Draw gravity vector
	ofSetColor(ofColor::green);
	ofDrawLine(0, 0, 0, xAcc, yAcc, zAcc);

	//ofPopMatrix();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'R')
		bSetupArduino = false;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (button == 2) {
		ofVec2f mouse(x, y);
		ofQuaternion yRot((x - lastMouse.x)*dampen, ofVec3f(0, 1, 0));
		ofQuaternion xRot((y - lastMouse.y)*dampen, ofVec3f(-1, 0, 0));
		curRot *= yRot*xRot;
		lastMouse = mouse;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	lastMouse = ofVec2f(x, y);
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
