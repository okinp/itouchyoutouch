
#include "testApp.h"



void testApp::setup() {
    ofSetFrameRate( 60 );
	mySensing = new sensing();

}
void testApp::update() {
	ofBackground( 100, 100, 100 );
	mySensing->update();


}
void testApp::draw() {
	mySensing->draw();

}

void testApp::keyPressed( int key ) {
    if( key == 'b' || key=='B' ) {
        mySensing->bLearnBakground = true;
    } else if( key == '-' ) {
        mySensing->threshold = MAX( 0, mySensing->threshold-1 );
    } else if( key == '+' || key == '=' ) {
        mySensing->threshold = MIN( 255, mySensing->threshold+1 );
    } else if (key =='f' || key=='F') {
		ofToggleFullscreen();
	}
}
void testApp::mouseMoved( int x, int y ) {}
void testApp::mouseDragged( int x, int y, int button ) {}
void testApp::mousePressed( int x, int y, int button ) {}
void testApp::mouseReleased() {}


void testApp::blobOn( int x, int y, int id, int order ) {
    cout << "blobOn() - id:" << id << " order:" << order << endl;
}
void testApp::blobMoved( int x, int y, int id, int order) {
    cout << "blobMoved() - id:" << id << " order:" << order << endl;

    // full access to blob object ( get a reference)
    ofxCvTrackedBlob blob = mySensing->blobTracker.getById( id );
    cout << "area: " << blob.area << endl;

}
void testApp::blobOff( int x, int y, int id, int order ) {
    cout << "blobOff() - id:" << id << " order:" << order << endl;
}



