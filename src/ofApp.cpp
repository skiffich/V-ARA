#include "ofApp.h"

// Project V/ARA
// 13/12/17
// Created by Dmytro Petrenko
// Short description
/* Short description
* Receive string from Arduino and convert string to int values
* Arduino connected to any COM port with baud rate 57600
* Arduino sends string using Firmata protocol when
* it receives "N" string using Firmata protocol
* Because when arduino will send string continuously,
* Program will stay in "String received handler" function
*/
// Instruction
/* Instruction
* Load to arduino sketch Arduino.ino from Arduino folder
*/
// Data
/* Data
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
// Errors & Exceptions
/* Errors & Exceptions:
* 1 (Solved) (12/12/17)
* Strange Exception in runtime caused by unknown what by and unknown where
* Exception: "vector iterator not dereferencable"
* Probably the reason is the non-sync threads in  "stringReceived" and "update"
* Result: I think it is fixed with adding delay between transactions to Arduino
* (22/12/17) adding try/catch to vector iterator from library "vector" func; line 52-56 (#if _ITERATOR_DEBUG_LEVEL == 2(...))
* 
* 2 (Solved)
* Exeption with string receivng
* Occurred when arduino sends string to COMport too
* Solved with adding try/catch and removing arduino string sending
*
* 3 (Solved)
* Didn't throws exception when bytes did not send to arduino
* (23/12/17) adding try/catch to SerialsendStrind func from library. It solves exeption at all
*
* 4
* Hand sample is roteded on 180 deg. I don't know on which axis
*/
// Updates
/* Updates
* 13/12/17
* Added 3D box primitive
* Added runtime calibration
* Added abbility for reconnection to arduino in runtime
* Fixed bug with orientation
*
* 17/12/17
* Added 3D hand model with textures
* Fixed bug with rotation in 3D
*
* 20/12/17
* Merdge with VARA1.0_BounceBalls
*
* 22/12/17
* Added Main menu
* Added ability to control with tensoresistors
*
* 24/12/17
* Added start screen
* Fixed bugs with tensoresistors
*/
// Features
/* Features
* Want to add ability to switch to mouse control mode from main menu 
*
* Want to add more signs
*/

//--------------------------------------------------------------
void ofApp::setup() {
	//******************************** Setup Mode SETUP ********************************
	clock_setup = clock() / CLOCKS_PER_SEC;
	setupCircleColors.push_back(ofColor(ofColor::black));
	setupCircleColors.push_back(ofColor(ofColor::lightGoldenRodYellow));
	setupCircleColors.push_back(ofColor(ofColor::fireBrick));
	setupCircleColors.push_back(ofColor(ofColor::forestGreen));
	setupCircleColors.push_back(ofColor(ofColor::crimson));
	setupCircleColors.push_back(ofColor(ofColor::bisque));
	setupCircleColors.push_back(ofColor(ofColor::honeyDew));
	setupCircleColors.push_back(ofColor(ofColor::red));
	setupCircleColors.push_back(ofColor(ofColor::moccasin));
	for (int i = 0; i < circleSetupCount; i++)
	{
		vector<double> v;
		angel += 360 / circleSetupCount;
		if (angel >= 360)angel = 0;
		v.push_back(radiusBigCircleSetup * cos(angel * DEG_TO_RAD));
		v.push_back(radiusBigCircleSetup * sin(angel * DEG_TO_RAD));
		setupCircleCoords.push_back(v);
	}
	radiusBigCircleSetup += 10;


	//******************************** 3D SETUP ********************************
	/*
	// light vectors for OpenGL
	GLfloat lightOnePosition[] = { 40.0, 40, 100.0, 0.0 };
	GLfloat lightOneColor[] = { 0.99, 0.99, 0.99, 1.0 };
	GLfloat lightTwoPosition[] = { -40.0, 40, 100.0, 0.0 };
	GLfloat lightTwoColor[] = { 0.99, 0.99, 0.99, 1.0 };

	ofDisableArbTex();
	//some model / light stuff
	ofEnableDepthTest();
	glShadeModel(GL_SMOOTH);
	// initialize lighting 
	glLightfv(GL_LIGHT0, GL_POSITION, lightOnePosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightOneColor);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT1, GL_POSITION, lightTwoPosition);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightTwoColor);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	*/
	//load the hand model - the 3ds and the texture file need to be in the same folder
	handModel.loadModel("Hand.3ds", 20);
	//some rotation
	handModel.setRotation(1, 90, 0, 1, 0);
	handModel.setScale(0.3, 0.3, 0.3);
	// texture for hand
	texture.loadImage("texture.jpg");



	//******************************** 2D SETUP ********************************
	srand(time(NULL));
	font.load("Pacifico.ttf", 18);
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
	balls[0].x = pX = 100;
	balls[0].y = pY = 100;

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



	//******************************** MENU SETUP ********************************
	menuRock.load("rock.png");
	fourFingersOKinMenu.load("4fingers.png");
	xCursor = ofGetWindowWidth() / 2;
	yCursor = ofGetWindowHeight() / 2;



	//******************************** ARDUINO SETUP ********************************
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
				//wasConnected = true;
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
	try
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
		sGyrXstream >> xGyr; zGyr *= kZ;
		std::string sGyrY = received.substr(18, 4);
		stringstream sGyrYstream(sGyrY);
		sGyrYstream >> yGyr; xGyr *= kX;
		std::string sGyrZ = received.substr(23, 4);
		stringstream sGyrZstream(sGyrZ);
		sGyrZstream >> zGyr; yGyr -= 180; zGyr *= kY;
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
	}
	catch (...)
	{
		ofLogError() << "invalid string position in string\t"<< string;
	}

	if (showInputStr) {
		//* Show converted values
		cout << xAcc << "\t" << yAcc << "\t" << zAcc << "\t" << xGyr << "\t" <<
			yGyr << "\t" << zGyr << "\t" << ten1 << "\t" << ten2 << "\t" <<
			ten3 << "\t" << ten4 << endl; //*/
	}
}

void ofApp::draw3D()
{
	ofPushMatrix();

	// Set a start point in the center of the screen
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 40);

	// Scaling by scrolling mouse
	cam.begin();

	// Rotate to the angle specified by the user
	ofVec3f axis;
	float angle;
	curRot.getRotate(angle, axis);
	ofRotate(angle, axis.x, axis.y, axis.z);

	// Draw coordinate system
	ofPushMatrix();
	ofRotateZ(90);
	ofSetColor(ofColor::blue);
	ofDrawGridPlane(25, 25);
	ofPopMatrix();
	ofPushMatrix();
	ofRotateY(90);
	ofSetColor(ofColor::yellow);
	ofDrawGridPlane(25, 25);
	ofPopMatrix();

	ofPushMatrix();

	// Move all on this pos
	//ofTranslate(300, 300, 300);
	ofRotateX(-55);

	// Rotate to the angle obtained from the gyroscope
	ofRotateX(xGyr);
	ofRotateY(yGyr);
	ofRotateZ(zGyr);

	// Draw hand wit texture
	ofSetColor(ofColor::white);
	texture.getTextureReference().bind();
	handModel.drawFaces();
	texture.getTextureReference().unbind();
	// Draw gravity vecror
	ofSetColor(ofColor::green);
	ofDrawLine(10, 10, 10, 10 + xAcc, 10 + yAcc, 10 + zAcc);

	ofPopMatrix();
	cam.end();
	ofPopMatrix();
}

void ofApp::draw2D()
{
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
	if (isWin)
	{
		font.drawString("For new game show", ofGetWindowWidth() / 2 - 50, ofGetWindowHeight() / 2 + 100);
		menuRock.draw(ofGetWindowWidth() / 2 - 50, ofGetWindowHeight() / 2 + 100, 0, 100, 100);
	}
	ofSetColor(ofColor::white);
}

void ofApp::drawMenu()
{
	ofBackgroundGradient(ofColor::lightGray, ofColor::lightSkyBlue, OF_GRADIENT_CIRCULAR);

	ofSetColor(menuMainColor);
	ofDrawCircle(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 70);
	ofSetColor(ofColor::white);
	fourFingersOKinMenu.draw(ofGetWindowWidth() / 2 - 40, ofGetWindowHeight() / 2 - 40, 0, 50, 50);
	font.drawString("OK", ofGetWindowWidth() / 2, ofGetWindowHeight() / 2 + 20);

	// Top
	ofSetColor(menuColorTop);
	ofDrawCircle(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2 - 160, 70);
	ofSetColor(ofColor::black);
	font.drawString("Bounce\nGame", ofGetWindowWidth() / 2 - 50, ofGetWindowHeight() / 2 - 180);
	// Bottom
	ofSetColor(menuColorBottom);
	ofDrawCircle(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2 + 160, 70);
	ofSetColor(ofColor::black);
	font.drawString("3D mode", ofGetWindowWidth() / 2 - 50, ofGetWindowHeight() / 2 + 160);
	// Right
	ofSetColor(menuColorRight);
	ofDrawCircle(ofGetWindowWidth() / 2 + 160, ofGetWindowHeight() / 2, 70);
	ofSetColor(ofColor::black);
	font.drawString("Minecraft", ofGetWindowWidth() / 2 + 160 - 50, ofGetWindowHeight() / 2);
	// Left
	ofSetColor(menuColorLeft);
	ofDrawCircle(ofGetWindowWidth() / 2 - 160, ofGetWindowHeight() / 2, 70);
	ofSetColor(ofColor::black);
	font.drawString("menu 4", ofGetWindowWidth() / 2 - 160 - 50, ofGetWindowHeight() / 2);

	ofSetColor(ofColor::red);
	ofDrawCircle(xCursor, yCursor, 8);
	ofSetColor(ofColor::white);
}

void ofApp::drawSetup()
{
	ofBackgroundGradient(ofColor::aqua, ofColor::blueSteel, OF_GRADIENT_CIRCULAR);

	font.drawString("Wait :)", ofGetWidth() / 2 - 50, ofGetHeight() / 2);

	double cur_clock_setup = (clock() / CLOCKS_PER_SEC) - clock_setup;
	//if (cur_clock_setup > 0){
	if (iColor == setupCircleColors.size() - 1)
	{
		_MODE_ = MENU_MODE;
		return;
	}

	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);

	ofSetColor(setupCircleColors[iColor]);
	angel += 360 / circleSetupCount;
	if (angel >= 360)angel = 0;
	setupCircleCoords[iCircle][0] = radiusBigCircleSetup * cos(angel * DEG_TO_RAD);
	setupCircleCoords[iCircle][1] = radiusBigCircleSetup * sin(angel * DEG_TO_RAD);
	iCircle++;
	ofSetColor(ofColor::white);

	ofPopMatrix();

	if (iCircle == circleSetupCount)
	{
		radiusBigCircleSetup += 10;
		iCircle = 0;
		iColor++;
	}

	clock_setup = clock() / CLOCKS_PER_SEC;
	//}

	for each (auto v in setupCircleCoords)
	{
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
		ofSetColor(setupCircleColors[iColor]);
		ofDrawCircle(v[0], v[1], radiusSmallCirclesSetup);
		ofSetColor(ofColor::white);
		ofPopMatrix();
	}
}

void ofApp::update3D()
{
	// return to menu
	if (ten1 < tensorSensivityMin && ten2 > tensorSensivityMax && ten3 > tensorSensivityMax) _MODE_ = MENU_MODE;
}

void ofApp::update2D()
{
	balls[0].x += (yGyr - pY)*10;
	balls[0].y += (xGyr - pX)*10;

	for (int i = 5; i < balls.size(); i++) {
		if (balls[i].color != balls[0].color)break;
		else if (i == balls.size() - 1 && !updated) { score++; updated = true; isWin = true; }
	}

	// for new game
	if (ten1 > tensorSensivityMax && ten2 > tensorSensivityMax && ten3 > tensorSensivityMax)
	{
		// Return the original colors
		updated = false;
		isWin = false;

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
			balls[0].color = color1;
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

	// return to menu
	if (ten1 < tensorSensivityMin && ten2 > tensorSensivityMax && ten3 > tensorSensivityMax) _MODE_ = MENU_MODE;

	if (isWin)
	{
		if (ten1 < 80 && ten2 < 80 && ten3 > 90)
		{
			isWin = false;
			// Return the original colors
			updated = false;
			isWin = false;

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
				balls[0].color = color1;
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
	}

	pX = xGyr;
	pY = yGyr;
}

void ofApp::updateMenu()
{
	xCursor += (yGyr - pY) * 10;
	yCursor += (xGyr - pX) * 10;

	if (xCursor > ofGetWindowWidth())xCursor = ofGetWindowWidth();
	if (yCursor > ofGetWindowHeight())yCursor = ofGetWindowHeight();
	if (xCursor < 0) xCursor = 0;
	if (yCursor < 0) yCursor = 0;

	int whH = ofGetWindowHeight()/2;
	int whW = ofGetWindowWidth()/2;

	int c1T = ofGetWindowHeight();

	// top
	if(		yCursor > whH - 230
		&&	yCursor < whH - 90
		&&	xCursor < whW + 70
		&&	xCursor > whW - 70)
	{
		menuColorTop = menuChoiseColor;
		if (ten1 > tensorSensivityMax && ten2 < tensorSensivityMin && ten3 < tensorSensivityMin)
			_MODE_ = Game2D_MODE;
	}
	else
		menuColorTop = menuMainColor;

	// bottom
	if (	yCursor < whH + 230
		&&	yCursor > whH + 90
		&&	xCursor < whW + 70
		&&	xCursor > whW - 70)
	{
		menuColorBottom = menuChoiseColor;
		if (ten1 > tensorSensivityMax && ten2 < tensorSensivityMin && ten3 < tensorSensivityMin)
			_MODE_ = Game3D_MODE;
	}
	else
		menuColorBottom = menuMainColor;

	// right
	if (	yCursor > whH - 70
		&&	yCursor < whH + 70
		&&	xCursor > whW + 90
		&&	xCursor < whW + 230)
	{
		menuColorRight = menuChoiseColor;
		if (ten1 > tensorSensivityMax && ten2 < tensorSensivityMin && ten3 < tensorSensivityMin)
			;
	}
	else
		menuColorRight = menuMainColor;

	// left
	if (	yCursor > whH - 70
		&&	yCursor < whH + 70
		&&	xCursor < whW - 90
		&&	xCursor > whW - 230)
	{
		menuColorLeft = menuChoiseColor;
		if (ten1 > tensorSensivityMax && ten2 < tensorSensivityMin && ten3 < tensorSensivityMin)
			;
	}
	else
		menuColorLeft = menuMainColor;

	pX = xGyr;
	pY = yGyr;
}

void ofApp::updateSetup()
{
}

//--------------------------------------------------------------
void ofApp::update() {
	// Send "N" to arduino
	// Calculate current clocks interval
	double cur_clock = double(clock() - clock_start) / CLOCKS_PER_SEC;
	// Update arduino if duration has been more than 0.05 seconds
	if (cur_clock > 0.1 && bSetupArduino) {
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
	else if (cur_clock > 0.1 && !bSetupArduino)
	{
		try
		{
			ard.disconnect();
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

	switch (_MODE_)
	{
	case Game3D_MODE:
		update3D();
		break;
	case Game2D_MODE:
		update2D();
		break;
	case MENU_MODE:
		updateMenu();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	switch (_MODE_)
	{
	case Game3D_MODE:
		draw3D();
		break;
	case Game2D_MODE:
		draw2D();
		break;
	case MENU_MODE:
		drawMenu();
		break;
	case Setup_MODE:
		drawSetup();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key)
	{
	case 'R': bSetupArduino = false; ofLogNotice() << "Reset..."; break;
	case 'C': ard.sendString("C"); ofLogNotice() << "Calibrating..."; break;
	case 'U': ard.sendString("U"); ofLogNotice() << "Mouse mode changed "; break;
	case 'X': if (_MODE_ == Game3D_MODE) { kX *= -1; ofLogNotice() << "Invert X"; break;}
	case 'Y': if (_MODE_ == Game3D_MODE) { kY *= -1; ofLogNotice() << "Invert Y"; break; }
	case 'Z': if (_MODE_ == Game3D_MODE) { kZ *= -1; ofLogNotice() << "Invert Z"; break; }
	case 'M': if (_MODE_ == Game2D_MODE) { _MODE_ = Game3D_MODE; ofClear(ofColor::white); }
			  else  { _MODE_ = Game2D_MODE; ofClear(ofColor::white); }
		ofLogNotice() << "Change game mode to "<< _MODE_; break;
	case 'S': showInputStr = !showInputStr; ofLogNotice() << "show Input String " << showInputStr; break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	switch (_MODE_)
	{
	case Game3D_MODE:
		if (button == 2) {
			ofVec2f mouse(x, y);
			ofQuaternion yRot((x - lastMouse.x)*dampen, ofVec3f(0, 1, 0));
			ofQuaternion xRot((y - lastMouse.y)*dampen, ofVec3f(-1, 0, 0));
			curRot *= yRot*xRot;
			lastMouse = mouse;
		}
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (_MODE_ == Game3D_MODE)
	{
		lastMouse = ofVec2f(x, y);
	}
	if (_MODE_ == Game2D_MODE)
	{
		// Return the original colors
		updated = false;
		isWin = false;

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
			balls[0].color = color1;
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
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
