
#include "testApp.h"



void testApp::setup() {
    ofSetFrameRate( 60 );
	ofBackground( 0, 0, 0 );
	mySensing = new sensing();

}
void testApp::update() {
	
	mySensing->update();


}
void testApp::draw() {
	mySensing->draw();

}

void testApp::keyPressed( int key ) {
    if( key == 'b' || key=='B' ) {
        mySensing->bLearnBakground = true;
    } else if (key =='f' || key=='F') {
		ofToggleFullscreen();
	} else if (key ==' ') {
		mySensing->show=!mySensing->show;
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



