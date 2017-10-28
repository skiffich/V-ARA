#pragma once
#include "ofMain.h"

class Ball
{
public:
	float spring = 0.03;
	float gravity = 0.03;
	float friction = -0.5;
	ofColor color;

	float x, y;
	float diameter;
	float vx = 0;
	float vy = 0;
	int id;
	vector<Ball> *others;

	Ball();
	~Ball();
	Ball(float xin, float yin, float din, int idin, vector<Ball> *oin, ofColor _color = ofColor::white);

	void collide();
	void move();
	void display();
};

