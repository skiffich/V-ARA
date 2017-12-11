#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// Hide console v1
	FreeConsole();

	font.load("Pacifico.ttf", 18);

	try
	{
		ard.connect("COM4", 57600);
		ofAddListener(ard.EStringReceived, this, &ofApp::stringReceived);
		isConnected = true;
	}
	catch(...)
	{
		isConnected = false;
	}

	srand(time(NULL));
	// Random player color
	int PlayerColor = rand() % 4;
	switch (PlayerColor)
	{
	case 0:
		balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color1));
		break;
	case 1:
		balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color2));
		break;
	case 2:
		balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color3));
		break;
	case 3:
		balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color4));
		break;
	}
	balls[0].x = 100;
	balls[0].y = 100;
	
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, ofColor::black));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, ofColor::black));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, ofColor::black));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, ofColor::black));

	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color1));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color1));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color2));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color2));
	
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color3));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color3));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color4));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color4));
	
	/*
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color5));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color5));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color6));
	balls.push_back(Ball(rand() % ofGetWindowWidth(), rand() % ofGetWindowHeight(), rand() % 40 + 30, balls.size(), &balls, color6));
	//*/
}

void ofApp::stringReceived(const string & string) {

	char sXacc[3];
	sXacc[0] = string[1];
	sXacc[1] = string[2];
	sXacc[2] = string[3];
	rX = atoi(sXacc);

	char sYacc[3];
	sYacc[0] = string[5];
	sYacc[1] = string[6];
	sYacc[2] = string[7];
	rY = atoi(sYacc);

	balls[0].x += rX;
	balls[0].y += rY;
}

//--------------------------------------------------------------
void ofApp::update(){
	for (int i = 5; i < balls.size(); i++) {
		if (balls[i].color != balls[0].color)break;
		else if (i == balls.size() - 1 && !updated) { score++; updated = true; }
	}

	if (isConnected && ard.isArduinoReady()) {
		try { ard.update(); }
		catch (...) { isConnected = false; }
	}
	else
	{
		balls[0].x = dX;
		balls[0].y = dY;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::aqua, ofColor::blueSteel, OF_GRADIENT_CIRCULAR);

	for (int i = 0; i < balls.size(); i++) {
		balls[i].collide();
		if (i > 4)
			balls[i].move();
		balls[i].display();
	}

	string text;
	stringstream ss;
	ss << score;
	text += "You winned " + ss.str() + " times";
	ofSetColor(ofColor::red);
	font.drawString(text, ofGetWindowWidth() / 2 - 50, ofGetWindowHeight() / 2);
	ofSetColor(ofColor::white);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	dX = x;
	dY = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	dX = x;
	dY = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	dX = x;
	dY = y;

	// Return the original colors
	updated = false;

	balls[5].color = color1;
	balls[6].color = color1;
	balls[7].color = color2;
	balls[8].color = color2;
	balls[9].color = color3;
	balls[10].color = color3;
	balls[11].color = color4;
	balls[12].color = color4;

	// Randomly set player's color
	int PlayerColor = rand() % 4;
	switch (PlayerColor)
	{
	case 0:
		balls[0].color= color1;
		break;
	case 1:
		balls[0].color = color2;
		break;
	case 2:
		balls[0].color = color3;
		break;
	case 3:
		balls[0].color = color4;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	dX = x;
	dY = y;
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
