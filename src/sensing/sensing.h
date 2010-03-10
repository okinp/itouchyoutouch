/*
 *  sensing.h
 *  itouchyoutouch
 *
 *  Created by Nikolas Psaroudakis on 3/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SENSING
#define _SENSING

/*     OF  Header       */
#include "ofMain.h"
/*     CV  Headers		*/
#include "ofxCvBlobTracker.h"
#include "ofxCvTrackedBlob.h"
#include "ofxCvConstants_Track.h"
/*    GUI				*/
#include "ofxSimpleGuiToo.h"
#include "ofxFBOTexture.h"
class sensing  {
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
#endif