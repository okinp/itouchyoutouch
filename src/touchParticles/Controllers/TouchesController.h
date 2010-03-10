#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "TouchController.h"
#include "ofxDirList.h"
#include "ofxVec2f.h"
#include "Constants.h"

class TouchesController
{
	public:
		
		TouchesController();
			
		void load();
		void update();
		void draw();
	
		void touchStarted(int blobid, vector <ofPoint> pts, ofPoint centroid);
		void touchMoved(int blobid, vector <ofPoint> pts, ofPoint centroid);
		void touchEnded(int blobid);
		
	private:
	
		void checkDistance();
		void updatePlaying();
		void showAllBut(int leaveOut, bool hideDrawing = false);
		void hideAllBut(int leaveOut, bool hideDrawing = false);
		void findClosest(int index);
		
		vector <TouchController*> touches;
		
		ofxVec2f compare1;
		ofxVec2f compare2;
	
		int numDrawing;
};