#pragma once

#include "ofMain.h"
#include "ofxCvBlobTracker.h"
#include "ofxCvTrackedBlob.h"
#include "ofxCvConstants_Track.h"
#include "ofxSimpleGuiToo.h"
#include "ofxFBOTexture.h"
#include "Constants.h"

class sensing  
{
	
public:
	int cwidth;
    int cheight;
	int threshold;
	bool bLearnBakground;
	bool show;
    ofVideoGrabber  vidGrabber;
    ofxCvColorImage  colorImg;
    ofxCvGrayscaleImage  grayImg;
    ofxCvGrayscaleImage  bgImg;
    ofxCvContourFinder  contourFinder;
    ofxCvBlobTracker  blobTracker;	
	//sensing();
	sensing(ofxCvBlobListener * listener);
	void update();
	void draw();
	vector <ofxCvTrackedBlob> getBlobs();
private:
	ofxFBOTexture outputTexture;
	int blurAmount;
	
	
};