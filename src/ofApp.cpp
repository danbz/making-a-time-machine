#include "ofApp.h"
#include "ofMath.h"
#include "ofxGui.h"
#include "ofxEasing.h"

float timeNow;
float timeLimit;
float randomNumber;
int loopNumber;
int loopMax;
int totalMovies;
int movieDuration;
int currentFrame;
bool playForward;

int vdoNr;
bool showGui;
bool xFading = false;
int xFadeProgress = 0;
 namespace fs = std::filesystem;
int fadeSpeed;


//easing stuff
auto duration = 5.f;
float initTime = 0;
auto endTime = initTime + duration;
bool leftToRight = true;
int startRange;
int endRange;
float endPosition;
float easedFrame;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0,0,0);
    ofSetVerticalSync(true);
    
    timeLimit = 4;// this represents the five seconds you want to set as a flag to call
    timeNow = 0;// always a good practice to define your variables in setup.
    loopNumber =0; // initialise loops to off
    loopMax=3; // max times to loop each movie - controllable via gui
    vdoNr=1; //starting random number for selecting movie
    totalMovies=11; //total amount of movies available to be played
    
    movieDuration=0;
    currentFrame=0;
    
    //build gui group
    gui.setup( "Parameters", "settings.xml" );
    gui.add( duration.setup( "duration", 5.0, 1.0, 10.0 ) );
    gui.add( loopMax.setup( "loopMax", 5, 1, 10 ) );
    gui.add( fadeSpeed.setup( "fadeSpeed", 5, 1, 10 ) );
    gui.add( videoAlpha.setup( "alpha", 255, 0, 255 ) );
    gui.add( fade.setup( "fade", false));
    //gui.add( palindrome.setup( "palindrome", false));
    showGui = true;
    momentMovie.setPixelFormat(OF_PIXELS_NATIVE);
    // CGDisplayHideCursor(kCGDirectMainDisplay);
    
    //--- easing stuff
    endPosition = ofGetWidth() - 40;
    easedFrame =0.f;
    startRange= 0;
    endRange = 0;
    
    this->loadNew();
}

//--------------------------------------------------------------
void ofApp::loadNew(){
    loopNumber =0; // initialise loops to zero
    vdoNr = (int) ofRandom(1, totalMovies);
    
    momentMovie.setPixelFormat(OF_PIXELS_NATIVE);
    
    vidImage.grabScreen(0,0, ofGetWidth(), ofGetHeight() ); // grab last frame of current video
    momentMovie.load("lge-movies/loop" + ofToString(vdoNr) +".mov"); //choose new clip randomly
     // momentMovie.load("lge-movies/sky-h264.mov"); //choose new clip for test comparison
    momentMovie.setLoopState(OF_LOOP_NONE);
    momentMovie.play();
    momentMovie.setSpeed(0);
    movieDuration = momentMovie.getTotalNumFrames();
    xFading = true; // set flag to show we should be xFading as a new clip has been loaded
    videoAlpha=0; // set alpha of video clip to 0 in preparation to xFade
    
    if (fade){
        xFade(); // if xfade is selected in gui then perform xfade.
    }
}

//--------------------------------------------------------------
void ofApp::xFade(){
    
        if (xFading) {
            ofEnableAlphaBlending();
            ofEnableBlendMode( OF_BLENDMODE_ALPHA );
            //
            ofSetColor( 255, 255 ); // draw the captured last frame of the previous video
            vidImage.draw( 0, 0, ofGetWidth(), ofGetHeight() );  // draw the captured frame to screen
            ofSetColor( 255, videoAlpha );
            
            //--- fade progression
            if (videoAlpha<255) {
                videoAlpha = videoAlpha+fadeSpeed;
                if (videoAlpha>255){videoAlpha=255;}
                }
            else {
                videoAlpha=255;
                xFading = false; // xfading complete
            }
        }
}

//--------------------------------------------------------------
void ofApp::update(){ 
    
//    if(momentMovie.getIsMovieDone()){ //old routine to swap movies on a timed basis
//        //-- std::cout << "loopDone: " << loopNumber << endl;
//        //--- hold last frame of previous loop of video to prevent flash of black -- maybe -- start
//        vidImage.grabScreen(0,0, ofGetWidth(), ofGetHeight() ); // grab last frame of current video
//        ofSetColor( 255, 255 ); // draw the captured last frame of the previosu video
//        vidImage.draw( 0, 0, ofGetWidth(), ofGetHeight() );
//        //--- hold last frame - end
        if (loopNumber>=loopMax){
            this->loadNew();
//        }else{
//            loopNumber ++;
//            momentMovie.load("lge-movies/lapse-" + ofToString(vdoNr) +".mov");
//            momentMovie.setLoopState(OF_LOOP_NONE);
//            momentMovie.play();
        }
//    } //end of old routine to swap movies
    
    momentMovie.update();

    if(playForward){
        if(currentFrame<movieDuration){
            currentFrame++;
        }else{
            playForward = false; //got to the end - turn round
            currentFrame--;
        }
    }else{
        if (currentFrame>0){
            currentFrame--;
        }else{
            playForward = true; //got to the beginning - turn round
            if (loopNumber>=loopMax){
                this->loadNew();
            } else{
                currentFrame++;
                loopNumber ++;
            }
        }
    }

   
}

//--------------------------------------------------------------
void ofApp::draw(){
    //if (blackGap == 0){
    // momentMovie.draw(0,0,ofGetWidth(),ofGetHeight()); //use to insert black space between each video
    //}
    // ofSetColor(ofColor::white);//write debug framerate to screen
    //  ofDrawBitmapString("value: " + ofToString(vdoNr), 10, 10);
    
    if (fade) xFade(); // if fade selected in gui then call xfade function
    momentMovie.draw(0,0,ofGetWidth(),ofGetHeight()); //draw frame of movie
    if ( showGui ) gui.draw();
    ofDrawBitmapString(ofGetFrameRate(),10,10);
    
    //-- draw easing fuctiuons
    ofSetColor(255);
    auto h = 20;
    auto y = 20;
    auto i = 0;
    //float newNow=0.f;
    float now = ofGetElapsedTimef();
    
    if (ofGetElapsedTimef() > endTime) {// start a loop left to right
        initTime = ofGetElapsedTimef();
        endTime = initTime + duration;
        leftToRight = !leftToRight;
        if (leftToRight){
            startRange= 0;
            endRange = movieDuration;
        } else {
            startRange= movieDuration;
            endRange = 0;
            loopNumber ++;
        }
    }
    
    easedFrame=ofxeasing::map_clamp(now, initTime, endTime, startRange, endRange, &ofxeasing::cubic::easeInOut);
    //ofDrawRectangle(easedFrame, 100, h, h); //start and ease in
    // cout << easedFrame ;
    momentMovie.setFrame(easedFrame);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' ') loadNew(); // show new loop video
    if ( key == 'g' ) showGui = !showGui; // show or hide the gui
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
