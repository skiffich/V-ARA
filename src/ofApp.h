#pragma once

#include "ofMain.h"
#include "ofEvents.h"

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

		// Arduino Firmata protocol worker
		ofArduino	ard;
		void stringReceived(const string & string);

		float	xAcc = 0,
				yAcc = 0,
				zAcc = 0,
				xGyr = 0,
				yGyr = 0,
				zGyr = 0,
				ten1 = 0,
				ten2 = 0,
				ten3 = 0,
				ten4 = 0;
		string received = "A000B000C000D0000E0000F0000G00H00I00J00K";
		
};
