#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	font.load("Pacifico.ttf", 18);

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

//--------------------------------------------------------------
void ofApp::update(){
	for (int i = 5; i < balls.size(); i++) {
		if (balls[i].color != balls[0].color)break;
		else if (i == balls.size() - 1 && !updated) { score++; updated = true; }
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
	balls[0].x = x;
	balls[0].y = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	balls[0].x = x;
	balls[0].y = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	balls[0].x = x;
	balls[0].y = y;

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
	balls[0].x = x;
	balls[0].y = y;
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
