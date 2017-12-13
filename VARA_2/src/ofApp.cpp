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
	ofDisableArbTex();
	ofEnableDepthTest();
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

	m_image.loadImage("texture.png");
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
	sAccXstream >> zAcc;
	zAcc *= 5 * kZ;
	std::string sAccY = received.substr(5, 3);
	stringstream sAccYstream(sAccY);
	sAccYstream >> xAcc;
	xAcc *= 5 * kX;
	std::string sAccZ = received.substr(9, 3);
	stringstream sAccZstream(sAccZ);
	sAccZstream >> yAcc;
	yAcc *= 5 * kY;
	// Convert gyroscope values
	std::string sGyrX = received.substr(13, 4);
	stringstream sGyrXstream(sGyrX);
	sGyrXstream >> zGyr; zGyr *= kZ;
	std::string sGyrY = received.substr(18, 4);
	stringstream sGyrYstream(sGyrY);
	sGyrYstream >> xGyr; zGyr *= kX;
	std::string sGyrZ = received.substr(23, 4);
	stringstream sGyrZstream(sGyrZ);
	sGyrZstream >> yGyr; yGyr -= 180; zGyr *= kY;
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
	cout << xAcc << "\t" << yAcc << "\t" << zAcc << "\t" << xGyr << "\t" <<
		yGyr << "\t" << zGyr << "\t" << ten1 << "\t" << ten2 << "\t" <<
		ten3 << "\t" << ten4 << endl; //*/
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

	

	// Draw coordinate system
	ofSetColor(ofColor::red);
	ofDrawLine(0, 0, 0, 300, 0, 0);
	ofSetColor(ofColor::blue);
	ofDrawLine(0, 0, 0, 0, 300, 0);
	ofSetColor(ofColor::yellow);
	ofDrawLine(0, 0, 0, 0, 0, 300);

	

	ofPushMatrix();

	ofTranslate(40, 40, 40);

	// Rotate to the angle obtained from the gyroscope
	ofRotateX(xGyr);
	ofRotateY(yGyr);
	ofRotateZ(zGyr);

	// Draw box wit texture
	ofSetColor(ofColor::white);
	m_image.getTextureReference().bind();
	ofDrawBox(0, 0, 0, 60, 40, 100);
	m_image.getTextureReference().unbind();
	// Draw gravity vecror
	ofSetColor(ofColor::green);
	ofDrawLine(10, 10, 10, 10 + xAcc, 10 + yAcc, 10 + zAcc);

	ofPopMatrix();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case 'R': bSetupArduino = false; ofLogNotice() << "Reset..."; break;
	case 'C': ard.sendString("C"); ofLogNotice() << "Calibrating..."; break;
	case 'X': kX *= -1; ofLogNotice() << "Invert X"; break;
	case 'Y': kY *= -1; ofLogNotice() << "Invert Y"; break;
	case 'Z': kZ *= -1; ofLogNotice() << "Invert Z"; break;
	}
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
