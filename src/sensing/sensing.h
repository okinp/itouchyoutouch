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
	
	ofxCvBlobTracker  blobTracker;	
	bool bLearnBakground;
	bool show;
	sensing(ofxCvBlobListener * listener);
	void update();
	void draw();
	vector <ofxCvTrackedBlob> getBlobs();
	
private:
	
	int cwidth;
    int cheight;
	int threshold;
	int minArea;
	
    ofVideoGrabber  vidGrabber;
    ofxCvColorImage  colorImg;
    ofxCvGrayscaleImage  grayImg;
    ofxCvGrayscaleImage  bgImg;
    ofxCvContourFinder  contourFinder;
	
	ofxFBOTexture outputTexture;
	int blurAmount;
	
	
};