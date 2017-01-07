/*
 * Joint.h
 *
 *  Created on: Jan 4, 2017
 *      Author: kevywilly
 */

#ifndef JOINT_H_
#define JOINT_H_

#define OPEN 0
#define CLOSED 1
#define NEUTRAL 2

#define POSITION_OPEN 100
#define POSITION_CLOSED 0

#include "Arduino.h"
#include <Servo.h>

class Joint {
public:

	// Constructor
	Joint(uint8_t pin, const int micros[3]) {
		_servo = new Servo;
		_pin = pin;
		_micros_closed = micros[0];
		_micros_open = micros[1];
		_micros_neutral = micros[2];
	}

	// Initialize Joint
	void init() {
		_servo->attach(_pin);
		//rest();
	}


	// Return a microseconds that correspond to an angle of the joint
	int position_to_micros(uint8_t position) {
		if(position < POSITION_CLOSED) {
			return position_to_micros(POSITION_CLOSED);
		}
		else if(position > POSITION_OPEN) {
			return position_to_micros(POSITION_OPEN);
		}

		return map(position, POSITION_CLOSED, POSITION_OPEN, _micros_closed, _micros_open);

	}

	// Returns micros for a given position mapped to min/max of our joint
	uint8_t micros_to_position(int micros) {
		return map(micros, _micros_closed, _micros_open, POSITION_CLOSED, POSITION_OPEN);
	}

	uint8_t currentPosition() {
		return micros_to_position(_micros);
	}

	// Release target angle
	inline void clearTarget() {
		_micros_target = _micros;
		_step = 0;
		_paused = false;
		_stopped = true;
	}

	void jumpToMicros(int micros) {
		_servo->writeMicroseconds(micros);
		_micros = micros;
	}

	void jumpToNeutral() {
		_micros_target = _micros_neutral;
		jumpToMicros(_micros_neutral);
	}

	void jumpToClosed() {
		_micros_target = _micros_closed;
		jumpToMicros(_micros_closed);
	}

	void jumpToOpen() {
		_micros_target = _micros_open;
		jumpToMicros(_micros_open);
	}

	void jumpToPosition(uint8_t position) {
		jumpToMicros(position_to_micros(position));
	}
	// Set target position 100% = open 0% = closed
	void setTarget(uint8_t position, uint8_t speed) {

		stop();

		int micros = position_to_micros(position);

		// what is the delta in degrees
		int delta = micros - _micros;
		int pos_delta = abs(delta);

		char sign = (delta == pos_delta) ? 1 : -1;

		int step = (pos_delta*speed)/100.0;

		_micros_target = micros;
		_step = sign * (step < 10 ? 10 : step);
	}

	bool targetReached() {
		return (_micros == _micros_target);
	}

	void move() {

		if(_paused)
			return;

		if(_micros == _micros_target) {
			return;
		}

		int new_micros = _micros + _step;

		if(_step > 0 && new_micros > _micros_target) {
			new_micros = _micros;
		} else if (_step < 0 && new_micros < _micros_target) {
			new_micros = _micros;
		}

		// execute the move
		if(new_micros != _micros) {
			_stopped = false;
			jumpToMicros(new_micros);

			delay(10);
		}

		if(_micros == _micros_target) {
			stop();
		} else {
			_stopped = false;
		}


	}

	// Returns true if joint is moving
	bool isMoving() {
		return !(_paused || _stopped);
	}

	bool isStopped() {
		return _stopped;
	}

	bool isPaused() {
		return _paused;
	}

	// Stop moving the joint and reset target
	inline void stop() {
		clearTarget();
	}

	void pause() {
		_paused = _stopped ? false : true;
	}

	void resume() {
		_paused = false;
	}

	void rest() {
		_servo->detach();
	}

//private:

	Servo *_servo;

	volatile bool _paused = false;
	volatile bool _stopped = true;

	uint8_t _pin;

	int _micros_target = 0;
	int _micros_open = 2400;
	int _micros_closed = 500;
	int _micros_neutral = (2400+500)/2;
	int _micros = _micros_neutral;

	int _step = 0;
	int8_t _speed = 0;
};

#endif /* JOINT_H_ */
