#pragma once

#include "ofMain.h"
#include "ofxGui.h"
//#include <ApplicationServices/ApplicationServices.h>
#include <string>
#include <iostream>
#include "ofxEasing.h"

class ofApp : public ofBaseApp{ 
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void loadNew();
    void xFade();
    void loadFiles();
    
    
    ofVideoPlayer 		momentMovie;
    ofImage             vidImage;
    
    ofxPanel gui;
    ofxFloatSlider duration;
    ofxIntSlider loopMax, videoAlpha, fadeSpeed;
    ofxToggle fade;
    
    //ofxGuiGroup primGroup;
    //ofxFloatSlider speed, length;
    //ofxVec2Slider size;
    //ofxColorSlider color;
    
    
    
};
