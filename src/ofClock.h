#ifndef _OF_CLOCK
#define _OF_CLOCK

#include "ofMain.h"

class ofClock {

public:
	ofClock( );

	void update( int, int , int );
	void draw( float, int, int);
    void setup();
        
private:
	float radius;

	int top;
	int left;

	float secondsAngle;
	float minutesAngle;
	float hoursAngle;
    ofColor clockBackGround;
    ofColor clockFace;
    ofColor clockHourHand;
    ofColor clockMinHand;
    ofColor clockSecHand;
    ofColor clockCentreDot;
    
}; 

#endif
