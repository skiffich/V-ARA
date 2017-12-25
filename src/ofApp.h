#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxAssimpModelLoader.h"
#include "../src/Ball.h"

#define MENU_MODE	0
#define Game3D_MODE 1
#define Game2D_MODE 2
#define Setup_MODE	3
#define Mouse_MODE	4

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

private:
	// Arduino Firmata protocol worker
	ofArduino	ard;
	void stringReceived(const string & string);
	// Delay between transactions to arduino
	clock_t clock_start;
	bool	bSetupArduino = false;
	bool	wasConnected = false;

	// Received data
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
	// Received string
	string received = "A000B000C000D0000E0000F0000G00H00I00J00K";
	// Convert received string to received data
	void convertReceivedString2ReceivedData(const string string);
	float kX = 1, kY = 1, kZ = 1;


	int _MODE_ = Setup_MODE;
	void draw3D();
	void draw2D();
	void drawMenu();
	void drawSetup();

	void update3D();
	void update2D();
	void updateMenu();
	void updateSetup();

	//**************************3D Mode*****************************
	// Draw and rotate coordinate system
	ofQuaternion curRot;
	ofVec2f lastMouse;
	float dampen = .4;

	// texture
	ofImage texture;

	ofxAssimpModelLoader handModel;

	ofEasyCam cam;

	bool showInputStr = false;

	int tensorSensivityMax = 90;
	int tensorSensivityMin = 40;


	//**************************2D Mode*****************************
	// Different collors for balls
	ofColor color1 = ofColor::yellow;
	ofColor color2 = ofColor::green;
	ofColor color3 = ofColor::blue;
	ofColor color4 = ofColor::red;
	ofColor color5 = ofColor::violet;
	ofColor color6 = ofColor::white;

	// array of balsl
	vector<Ball> balls;

	// user ball offset
	int dX = 0, dY = 0;
	int pX = 0, pY = 0;

	// number of wins
	int score = 0;
	bool updated = false;
	bool isWin = false;
	// Font for showing score
	ofTrueTypeFont	font;



	//**************************Setup Mode*****************************
	vector<ofColor> setupCircleColors;
	vector<vector<double>> setupCircleCoords;
	double clock_setup;
	int iColor = 0;
	int iCircle = 0;
	double angel = 0;
	int circleSetupCount = 20;
	int radiusBigCircleSetup = 10, radiusSmallCirclesSetup = 5;


	//**************************MENU Mode*****************************
	ofImage menuRock;
	ofImage fourFingersOKinMenu;

	ofColor menuMainColor = ofColor::gray;
	ofColor menuColorTop = ofColor::gray;
	ofColor menuColorBottom = ofColor::gray;
	ofColor menuColorLeft = ofColor::gray;
	ofColor menuColorRight = ofColor::gray;
	ofColor menuChoiseColor = ofColor::lightGreen;

	int xCursor, yCursor;
	int pxCursor, pyCursor;
};
