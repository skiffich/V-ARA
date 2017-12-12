#include "ofApp.h"

// 12/12/17
// Receive string when press mouse button
// Arduino connected to COM5 with baud rate 57600
// Arduino sends string using Firmata protocol when
// it receives "N" string using Firmata protocol
// Because when arduino will send string continuously,
// Program will stay in "String received handler" function
//
// Load to arduino sketch Arduino.ino from Arduino folder

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
	// Show received string in console
	cout << string << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	// Apdate arduino
	ard.update();
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
	// Send "N" to arduino
	ard.sendString("N");
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
