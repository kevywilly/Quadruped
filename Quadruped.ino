#include "Arduino.h"
#include "Joint.h"
#include "Leg.h"


#define LEGS 4

#define CLOSED 0
#define OPEN 100

volatile bool stopped = false;

#define LEG1PINS {2,3,4}
#define LEG1MICROS {{1200,2000,1600},{1200,2000,1600},{1200,2000,1600}}

#define LEG2PINS {5,6,7}
#define LEG2MICROS {{1750,950,1350},{1750,950,1350},{1750,950,1350}}

#define LEG3PINS {8,9,10}
#define LEG3MICROS {{1050,1850,1450},{1000,1800,1400},{1000,1800,1400}}

#define LEG4PINS {11,12,13}
#define LEG4MICROS {{1800,1000,1400},{1750,950,1350},{1750,950,1350}}

#define LEG1 legs[0]
#define LEG2 legs[1]
#define LEG3 legs[2]
#define LEG4 legs[3]

enum Position {
	closed = 0,
	middle = 50,
    open = 100
};

const uint8_t leg1_pins[3] = LEG1PINS;
const uint8_t leg2_pins[3] = LEG2PINS;
const uint8_t leg3_pins[3] = LEG3PINS;
const uint8_t leg4_pins[3] = LEG4PINS;

const int leg1_micros[3][3] = LEG1MICROS;
const int leg2_micros[3][3] = LEG2MICROS;
const int leg3_micros[3][3] = LEG3MICROS;
const int leg4_micros[3][3] = LEG4MICROS;


Leg* legs[LEGS] {new Leg(leg1_pins, leg1_micros), new Leg(leg2_pins, leg2_micros), new Leg(leg3_pins, leg3_micros), new Leg(leg4_pins, leg4_micros)};

#define NUM_PATHS 6

#define P1 {{0,0,0},{0,0,0},{0,0,0},{0,0,0}}
#define P2 {{0,0,0},{100,0,0},{100,0,0},{100,0,0}}
#define P3 {{50,0,0},{50,0,0},{50,0,0},{50,0,0}}
#define P4 {{0,0,0},{100,0,0},{0,0,0},{100,0,0}}
#define P5 {{100,0,0},{0,0,0},{100,0,0},{0,0,0}}
#define P6 {{50,0,0},{50,0,0},{50,0,0},{50,0,0}}

const uint8_t paths[NUM_PATHS][4][3] = {P1,P2,P3,P4,P5,P6};

const uint8_t speed = 20;
int currentPath = 0;


//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(115200);

	// initialize legs
	for(int i=0; i < LEGS; i++) {
		legs[i]->init();
	}

	delay(100);

	for(int i=0; i < LEGS; i++) {
		//legs[i]->relax();
	}

	printStatus();

}

// The loop function is called in an endless loop
void loop()
{
	move(); //Add your repeated code here
}

void getNextPath() {
	if(currentPath > (NUM_PATHS-1)) {
		currentPath = 0;
	}

	for(int i=0; i<4; i++) {
		legs[i]->moveTo(paths[currentPath][i], speed);
	}

	delay(500);
	currentPath++;
}

void move() {
	if(stopped) {
		return;
	}

	if(targetsReached()) {
			getNextPath();
		}

	for(int i=0; i < LEGS; i++) {
		legs[i]->move();
		delay(5);
	}

}

bool targetsReached() {
	for(int i=0; i < LEGS; i++) {
		if(!legs[i]->targetReached()) {
			return false;
		}
	}
	return true;
}

void printStatus() {
	Serial.println("---------------------------");
	for(int i=0; i < LEGS; i++) {
		Serial.print("Leg: ");
		Serial.print(i);
		Serial.print(" = ");
		Serial.print(legs[i]->joints[0]->_micros);
		Serial.print(", ");
		Serial.print(legs[i]->joints[0]->_micros_target);
		Serial.print(", ");
		Serial.println(legs[i]->joints[0]->_step);
	}
}
