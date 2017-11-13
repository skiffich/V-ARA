#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "../Ball.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofColor color1 = ofColor::yellow;
		ofColor color2 = ofColor::green;
		ofColor color3 = ofColor::blue;
		ofColor color4 = ofColor::red;
		ofColor color5 = ofColor::violet;
		ofColor color6 = ofColor::white;

		vector<Ball> balls;

		int dX = 0, dY = 0;

		// number of wins
		int score = 0;
		bool updated = false;
		ofTrueTypeFont	font;
		

		// Firmata Arduino protocol
		ofArduino	ard;
		void stringReceived(const string & string);
		int rX = 0, rY = 0;
		bool isConnected = false;
};