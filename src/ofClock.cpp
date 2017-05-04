#include "ofClock.h"

ofClock::ofClock()
{
	//Make everything looking nice and smooth.
	ofSetCircleResolution(100);
	ofEnableSmoothing();

	//Set size & position of our clock
	if( ofGetHeight() < ofGetWidth() )
		radius = (ofGetHeight()/2)-20;
	else
		radius = (ofGetWidth()/2)-20;
	
	top = (ofGetHeight()/2);
	left = (ofGetWidth()/2);
}

void ofClock::update( int sec, int min, int hour ){
    
	secondsAngle = 6 * sec;
	minutesAngle = 6 * min;
	hoursAngle = 30 * hour + (min / 2);
}

void ofClock::setup(){
     clockBackGround = (200, 50);
     clockFace = 160;
     clockHourHand = 255;
     clockMinHand =255;
     clockSecHand = (255,0,0);
    clockCentreDot = 0xFF9999;
}

void ofClock::draw( float radius, int left, int top){
    
	//Set the coortinatesystem to the center of the clock. 
	ofPoint circle_center = ofPoint( left, top );
	ofTranslate(circle_center);
	ofRotateZ(-90);
    
	//Draw background of the clock
	//ofSetHexColor(0xbbbbbb);
//    ofSetColor(clockBackGround);
//	ofFill();
//	ofDrawCircle( ofPoint(0,0), radius );
    
	//Draw Outline of the clock
	ofSetLineWidth( (radius/100)*4 );
	ofNoFill();
	//ofSetHexColor(0x888888);
    ofSetColor(clockFace);
	ofDrawCircle( ofPoint(0,0), radius );
    
	//Draw the clock face
	ofSetLineWidth(1);
    
	for(int i=0;i<12;i++){
		ofRotateZ( 30 );
		
		ofFill();
		ofDrawRectangle( ofPoint( (radius/10)*8, -((radius/100)*2) ), (radius/100)*20, (radius/100)*4 );
		ofNoFill();
		ofDrawRectangle( ofPoint( (radius/10)*8, -((radius/100)*2) ), (radius/100)*20, (radius/100)*4 );
	}
    
	//Draw the hour hand
	ofPushMatrix(); 
	
	//ofSetHexColor(0xFF0000);
    ofSetColor(clockHourHand);
	ofRotateZ( hoursAngle );
	
	ofFill();
	ofDrawRectangle( 0,-((radius/100)*5),(radius/10)*6, (radius/100)*10 );
	ofNoFill();
	ofDrawRectangle( 0,-((radius/100)*5),(radius/10)*6, (radius/100)*10 );
	
	ofPopMatrix();
	
	//Draw the minute hand
	ofPushMatrix();
    
	//ofSetHexColor(0xFF6666);
    ofSetColor(clockMinHand);
    ofRotateZ( minutesAngle );
	
	ofFill();
	ofDrawRectangle( 0,-((radius/100)*4),(radius/10)*7, (radius/100)*8 );
	ofNoFill();	
	ofDrawRectangle( 0,-((radius/100)*4),(radius/10)*7, (radius/100)*8 );
    
	ofPopMatrix();
	
	//Draw the second hand
//	ofPushMatrix();
//	
//	//ofSetHexColor(0xFF9999);
//    ofSetColor(clockSecHand);
//	ofRotateZ( secondsAngle );
//	
//	ofFill();
//	ofDrawRectangle( -((radius/100)*2),-((radius/100)*2),(radius/10)*9, (radius/100)*4 );
//	ofNoFill();	
//	ofDrawRectangle( -((radius/100)*2),-((radius/100)*2),(radius/10)*9, (radius/100)*4 );
//    
//	ofPopMatrix();
	
	ofFill();
	ofSetHexColor(0xFF9999);
    ofSetColor(clockCentreDot);
	ofDrawCircle( ofPoint(0,0), (radius/100)*6 );
    ofSetColor(255);
}
