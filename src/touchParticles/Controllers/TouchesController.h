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
		void update(float mouseX, float mouseY);
		void draw();
	
		void touchStarted(int blobid);
		void touchEnded();
		
	private:
	
		void checkDistance();
		void updatePlaying();
		void showAllBut(int leaveOut, bool hideDrawing = false);
		void hideAllBut(int leaveOut, bool hideDrawing = false);
		
		vector <TouchController*> touches;
		
		int _drawing;
		int _playing;
		
		ofxVec2f compare1;
		ofxVec2f compare2;
};