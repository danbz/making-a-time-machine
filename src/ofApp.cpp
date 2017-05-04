#include "ofApp.h"
#include "ofMath.h"
#include "ofxGui.h"
#include "ofxEasing.h"
#include "ofUtils.cpp"
#include "ofClock.h"
#include <ctime>

float timeNow;
float timeLimit;
int loopNumber;
int loopMax;
int totalMovies;
int movieDuration;
int currentFrame;

int vdoNr;
bool showGui;
bool showClock;
bool xFading = false;
int xFadeProgress = 0;
 namespace fs = std::filesystem;
int fadeSpeed;


//easing stuff
auto duration = 5.f;
float initTime = 0;
auto endTime = initTime + duration;
bool playForward = true;
int startRange;
int endRange;
int endPosition;
int easedFrame;
int numOfFiles;

//-- clock stuff
float clockRadius;
int clockPosLeft;
int clockPosTop;

int clockSec;
int clockMin;
int clockHrs;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0,0,0);
    ofSetVerticalSync(true);
    
    timeLimit = 4;// this represents the five seconds you want to set as a flag to call
    timeNow = 0;// always a good practice to define your variables in setup.
    loopNumber =0; // initialise loops to off
    loopMax=3; // max times to loop each movie - controllable via gui
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
    showClock = true;
    playForward = true;
    
    momentMovie.setPixelFormat(OF_PIXELS_NATIVE);
    // CGDisplayHideCursor(kCGDirectMainDisplay);
    
    //--- easing stuff
    endPosition = ofGetWidth() - 40;
    easedFrame =0.f;
    startRange= 0;
    endRange = 0;
    
    //ofRestoreWorkingDirectoryToDefault();
    cout << "dataPathRoot(): " <<  dataPathRoot()  << endl; // prints-> dataPathRoot(): ../../../data/

    //read directory for number of files
    files.allowExt("mov");
    //string path = "/Users/danbuzzo/Desktop/lapses";
    string path = "movies"; //relative to /bin/data folder
    
    files.listDir(path); // put a video path here with several video files in a folder
    std::cout << "number of files" << files.size() << endl;
     numOfFiles = files.size() & INT_MAX;;
    this->loadNew();
    
    // -- clock setup
    clockRadius = 60.0;
    clockPosLeft = ofGetWidth()-110;
    clockPosTop = ofGetHeight()-110;;
    clock.setup();
    
}

//--------------------------------------------------------------
void ofApp::loadNew(){
    loopNumber =0; // initialise loops to zero
    
    momentMovie.setPixelFormat(OF_PIXELS_NATIVE);
    
    vidImage.grabScreen(0,0, ofGetWidth(), ofGetHeight() ); // grab last frame of current video
    string newMovie = files.getPath(ofRandom(numOfFiles));
    std::cout << newMovie <<endl;
    momentMovie.load(newMovie);
    
    momentMovie.setLoopState(OF_LOOP_NONE);

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

        if (loopNumber>=loopMax){
            this->loadNew();
        }

    momentMovie.update();

   //--clock updates
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    clockSec = now->tm_sec;
    //  clockMin = now->tm_min;
    clockMin = easedFrame % 60;
    clockHrs = easedFrame/60;
    // clockHrs = now->tm_hour;
    clock.update(clockSec, clockMin, clockHrs);

}

//--------------------------------------------------------------
void ofApp::draw(){
    
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
        playForward = !playForward;
        if (playForward){
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
    
    if (showClock)clock.draw( clockRadius, clockPosLeft, clockPosTop );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' ') loadNew(); // show new loop video
    if ( key == 'g' ) showGui = !showGui; // show or hide the gui
    if ( key =='c' ) showClock = !showClock; //show of hide the clock
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
