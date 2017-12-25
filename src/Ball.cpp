#include "Ball.h"


Ball::Ball()
{
	spring = 0.03;
	gravity = 0.03;
	friction = -0.5;
	vx = 0;
	vy = 0;
}

Ball::~Ball()
{
}

Ball::Ball(float xin, float yin, float din, int idin, vector<Ball> *oin, ofColor _color)
{
	x = xin;
	y = yin;
	diameter = din;
	id = idin;
	others = oin;
	color = _color;
}

void Ball::collide() {
	for (int i = id + 1; i < (*others).size(); i++) {
		float dx = (*others)[i].x - x;
		float dy = (*others)[i].y - y;
		float distance = sqrt(dx*dx + dy*dy);
		float minDist = (*others)[i].diameter / 2 + diameter / 2;
		if (distance < minDist) {
			float angle = atan2(dy, dx);
			float targetX = x + cos(angle) * minDist;
			float targetY = y + sin(angle) * minDist;
			float ax = (targetX - (*others)[i].x) * spring;
			float ay = (targetY - (*others)[i].y) * spring;
			vx -= ax;
			vy -= ay;
			(*others)[i].vx += ax;
			(*others)[i].vy += ay;

			if (id > 4)
				if (abs(vx) + abs(vy) > abs((*others)[i].vx) + abs((*others)[i].vy))
					(*others)[i].color = color;
				else
					color = (*others)[i].color;
		}
	}
}

void Ball::move() {
	//vy += gravity;
	x += vx;
	y += vy;
	if (x + diameter / 2 > ofGetWindowWidth()) {
		x = ofGetWindowWidth() - diameter / 2;
		vx *= friction;
	}
	else if (x - diameter / 2 < 0) {
		x = diameter / 2;
		vx *= friction;
	}
	if (y + diameter / 2 > ofGetWindowHeight()) {
		y = ofGetWindowHeight() - diameter / 2;
		vy *= friction;
	}
	else if (y - diameter / 2 < 0) {
		y = diameter / 2;
		vy *= friction;
	}
}

void Ball::display() {
	if ((*others)[0].x < 0)(*others)[0].x = 0;
	if ((*others)[0].y < 0)(*others)[0].y = 0;
	if ((*others)[0].x > ofGetWindowWidth()) (*others)[0].x = ofGetWindowWidth();
	if ((*others)[0].y > ofGetWindowHeight()) (*others)[0].y = ofGetWindowHeight();
	ofSetColor(this->color);
	ofDrawEllipse(x, y, diameter, diameter);
	ofSetColor(ofColor::white);
}
