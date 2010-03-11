
#include "testApp.h"

void testApp::setup() 
{
	ofSetFrameRate(60);
	ofBackground( 0, 0, 0 );
	
	mySensing = new sensing(this);
	
	touches = new TouchesController();
	touches->load();
}

void testApp::update() 
{	
	mySensing->update();
	
	for(int i = 0; i < mySensing->blobTracker.blobs.size(); i++)
	{
		// if this ever fucks up, it's because ids arent the same as vector index
		ofxCvTrackedBlob& blob = mySensing->blobTracker.blobs[i];
		touches->touchMoved(blob.id, blob.pts, blob.centroid);
	}
	
	touches->update();
}

void testApp::draw() 
{	
	mySensing->draw();
	touches->draw();
}

void testApp::keyPressed( int key ) 
{
    if( key == 'b' || key=='B' ) 
	{
        mySensing->bLearnBakground = true;
    } 
	else if (key =='f' || key=='F') 
	{
		ofToggleFullscreen();
	} 
	else if (key ==' ') 
	{
		mySensing->show =! mySensing->show;
	}
}

void testApp::mouseMoved( int x, int y ) {}
void testApp::mouseDragged( int x, int y, int button ) {}
void testApp::mousePressed( int x, int y, int button ) {}
void testApp::mouseReleased() {}


void testApp::blobOn( int x, int y, int id, int order ) 
{
	if(!mySensing->disabled)
	{		
		ofxCvTrackedBlob blob = mySensing->blobTracker.getById(id);
		
		touches->touchStarted(blob.id, blob.pts, blob.centroid);	
	}
}

void testApp::blobMoved( int x, int y, int id, int order)
{
	/*if(!mySensing->disabled)
	{	
		ofxCvTrackedBlob blob = mySensing->blobTracker.getById( id );
	
		touches->touchMoved(blob.id, blob.pts, blob.centroid);
	}*/
}
void testApp::blobOff( int x, int y, int id, int order ) 
{	
	if(!mySensing->disabled)
	{
		touches->touchEnded(id);
	}
}



