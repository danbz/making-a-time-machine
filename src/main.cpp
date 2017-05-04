#include "ofMain.h"
#include "ofApp.h"
#include "ofClock.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1080,720,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    
    
    //ofSetupOpenGL(768,576,OF_FULLSCREEN);			// <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    
    // added for pi
    // ref https://forum.openframeworks.cc/t/h264-mp4-video-not-playing-on-raspberry-pi-with-ofvideoplayer/21787/7
    
    
    //ofSetLogLevel(OF_LOG_VERBOSE);
//    ofGLESWindowSettings settings;
//    settings.width = 1280;
//    settings.height = 720;
//    settings.setGLESVersion(2);
//    ofCreateWindow(settings);

	ofRunApp(new ofApp()); 

}
