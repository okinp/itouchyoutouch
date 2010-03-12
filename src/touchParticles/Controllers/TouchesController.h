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
		
		void reset();
		
	private:
	
		void loadTextures();
		void checkDistance();
		void updatePlaying();
	
		void findClosest(int index);
		void showFurthestAway(int index);	
	
		void connectionMade(int drawing, int playing);
		void connectionStopped(int drawing, int playing);
		
		int getNextTexture();
		
		vector <TouchController*> touches;
		
		ofxVec2f compare1;
		ofxVec2f compare2;
	
		int numDrawing;
		int outlineScale;
	
		ofImage textures[NUM_TEXTURES];
	
		int curTexture;
};