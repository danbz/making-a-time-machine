#include "ofApp.h"
#include "ofMath.h"
#include "ofxGui.h"


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

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0,0,0);
    ofSetVerticalSync(true);
    
    timeLimit = 4;// this represents the five seconds you want to set as a flag to call
    timeNow = 0;// always a good practice to define your variables in setup.
    loopNumber =0; // initialise loops to off
    loopMax=3; // max times to loop each movie - controllable via gui
    vdoNr=1; //starting random number for selecting movie
    totalMovies=80; //total amount of movies available to be played
    
    movieDuration=0;
    currentFrame=0;
    playForward=true;
    
    //build gui group
    gui.setup( "Parameters", "settings.xml" );
    gui.add( speed.setup( "speed", 1.0, -10.0, 10.0 ) );
    gui.add( loopMax.setup( "loopMax", 5, 1, 10 ) );
    gui.add( videoAlpha.setup( "alpha", 255, 0, 255 ) );
    gui.add( fade.setup( "fade", false));
    gui.add( palindrome.setup( "palindrome", false));
    showGui = true;
    
    
    this->loadNew();
    // CGDisplayHideCursor(kCGDirectMainDisplay);
}

//--------------------------------------------------------------

void ofApp::loadFiles() {
   //unused routine that may parse directory and automatically read in number and names of movie files
        fs::create_directories("sandbox/a/b");
        std::ofstream("sandbox/file1.txt");
        std::ofstream("sandbox/file2.txt");
        for(auto& p: fs::directory_iterator("sandbox"))
        std::cout << p << '\n';
        fs::remove_all("sandbox");
    
}

//--------------------------------------------------------------

void ofApp::loadNew(){
    loopNumber =0; // initialise loops to zero
    vdoNr = (int) ofRandom(1, totalMovies);
    vidImage.grabScreen(0,0, ofGetWidth(), ofGetHeight() ); // grab last frame of current video
    // std::cout << "value: " << vdoNr << endl;
    momentMovie.load("lge-movies/lapse-" + ofToString(vdoNr) +".mov"); //choose new clip randomly
    movieDuration = momentMovie.getTotalNumFrames();
    if (palindrome) {
        momentMovie.setLoopState(OF_LOOP_PALINDROME);
    } else {
        momentMovie.setLoopState(OF_LOOP_NONE);
    }
    momentMovie.play();
    momentMovie.setSpeed(speed);
    //--- std::cout << "speed: " << speed << endl;
    xFading = true; // set flag to show we should be xFading as a new clip has been loaded
    videoAlpha=0; // set alpha of video clip to 0 in preparation to xFade
    
    if (fade){
        xFade(); // if xfade is selected in gui then perform xfade.
    }
}

//--------------------------------------------------------------

void ofApp::xFade(){
    
    //if (fade){
        // std::cout << "fade: " << endl;
        ofEnableAlphaBlending();
        ofEnableBlendMode( OF_BLENDMODE_ALPHA );
        
        if (xFading) {
            ofSetColor( 255, 255 ); // draw the captured last frame of the previous video
            vidImage.draw( 0, 0, ofGetWidth(), ofGetHeight() );  // draw the captured frame to screen
            ofSetColor( 255, videoAlpha );
            
            //--- fade progression
            if (videoAlpha<255) {
                videoAlpha = videoAlpha+1;
            } else {
                videoAlpha=255;
                xFading = false; // xfading complete
            }
        }
    //} else {
    //   ofSetColor( 255, 255 );
    //}
}

//--------------------------------------------------------------
void ofApp::update(){ 
    momentMovie.update();
    
//    if(momentMovie.getIsMovieDone()){
//        //-- std::cout << "loopDone: " << loopNumber << endl;
//        //--- hold last frame of previous loop of video to prevent flash of black -- maybe -- start
//        vidImage.grabScreen(0,0, ofGetWidth(), ofGetHeight() ); // grab last frame of current video
//        ofSetColor( 255, 255 ); // draw the captured last frame of the previosu video
//        vidImage.draw( 0, 0, ofGetWidth(), ofGetHeight() );
//        //--- hold last frame - end
//        if (loopNumber>=loopMax){this->loadNew();
//            
//        }else{
//            loopNumber ++;
//            momentMovie.load("lge-movies/lapse-" + ofToString(vdoNr) +".mov");
//            momentMovie.setLoopState(OF_LOOP_NONE);
//            momentMovie.play();
//        }
//    }
    
    //--------
    //new palindrome looping ting
    //
    //momentMovie.getCurrentFrame() = thisFrame;
    if(playForward){
        if(currentFrame<movieDuration){
            currentFrame++;
        }else{
            playForward = false; //got to the end - turn round
            currentFrame--;
             //momentMovie.setSpeed(-1.0);
        }
    }else{
        if (currentFrame>0){
            currentFrame--;
        }else{
            playForward = true; //got to the beginning - turn round
            currentFrame++;
            //momentMovie.setSpeed(1.0);
        }
    }
    
    //std::cout << "palindrome frame: " << currentFrame << endl;
    //std::cout << "movieDuration: " << movieDuration << endl;
    //std::cout << "playForward: " << playForward << endl;

    momentMovie.setFrame(currentFrame);
    
    //
    //-------
    //
    
    //timeNow = ofGetElapsedTimef();// this functions keep track of the time in seconds once the application has initialized.
    
    //    if ( timeNow >= timeLimit){//if the current time is equal or greater than 3.2 seconds...
    //      //loadNew();//we exit the LOADNEW function.
    //      timeLimit=timeNow + gapValue;
    //      //randomNumber = ofRandom(0, .995);
    //      //std::cout << "value: " << randomNumber << endl;
    //        if (blackGap==0){
    //            blackGap=1;
    //            gapValue=15;
    //        } else {
    //            blackGap=0;
    //            gapValue=15;
    //        }
    //        this->loadNew();
    //      //momentMovie.setPosition(randomNumber);
    //      ofHideCursor();
    //    }
    
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
