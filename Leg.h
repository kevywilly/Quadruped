
/*
 * Leg.h
 *
 *  Created on: Dec 20, 2016
 *      Author: kevywilly
 */

#ifndef LEG_H_
#define LEG_H_

#define PELVIS 0
#define HIP 1
#define KNEE 2

#include "joint.h"

#define J0 joints[0]
#define J1 joints[1]
#define j2 joints[2]

class Leg {

public:

	Joint *joints[3];

	// Construct leg and initialize the joints
	Leg(const uint8_t pins[3], const int micros[][3]) {
		for(int i; i < 3; i++) {
			joints[i] = new Joint(pins[i], micros[i]);
		}
	}

	// initialize each joint
	void init() {
		for(int i; i < 3; i++) {
			joints[i]->init();
			joints[i]->jumpToNeutral();
		}
	}

	void relax() {
		for(int i; i < 3; i++) {
			joints[i]->rest();
		}
	}
	void begin() {

	}

	void moveJointTo(uint8_t joint, uint8_t angle, uint8_t speed) {
		joints[joint]->setTarget(angle, speed);
	}

	void moveTo(const uint8_t angles[3], const uint8_t speed) {
		for(int i=0; i < 4; i++) {
			joints[i]->setTarget(angles[i], speed);
		}
	}

	void move() {
		for(int i=0; i < 3; i++) {
			joints[i]->move();
		}
	}

	bool isMoving() {
		for(int i; i < 3; i++) {
			if(joints[i]->isMoving()) {
				return true;
			}
		}

		return false;
	}

	bool targetReached() {
		for(int i; i < 3; i++) {
			if(!joints[i]->targetReached()) {
				return false;
			}
		}
		return true;
	}

private:




};

#endif /* LEG_H_ */
