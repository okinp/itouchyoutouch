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
	
	void update();
	void draw();
	sensing(ofxCvBlobListener * listener);
	int cwidth;
    int cheight;
	int threshold;
	bool bLearnBakground;
	bool show;
	int area;
    ofVideoGrabber  vidGrabber;
    ofxCvColorImage  colorImg;
    ofxCvGrayscaleImage  grayImg;
    ofxCvGrayscaleImage  bgImg;
    ofxCvContourFinder  contourFinder;
	
	ofxCvBlobTracker blobTracker;
	ofxFBOTexture outputTexture;
	int blurAmount;
	bool disabled;
};