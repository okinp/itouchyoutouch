#include "TouchesController.h"

/* Constructor
 ___________________________________________________________ */

TouchesController::TouchesController()
{
	numDrawing = 0;
	
	outlineScale = ofGetWidth() / VIDEO_WIDTH;
	int diff = (ofGetHeight() / VIDEO_HEIGHT) - outlineScale;
	
	if(diff != 0.0)
	{
		printf("ERROR: Width and height scale is wrong: %f \n", diff);
	}
}

/* Load
 ___________________________________________________________ */

void TouchesController::load()
{
	ofxDirList DIR;
	DIR.allowExt("xml");
	
	int numFiles = DIR.listDir(FOLDER);
	
	for(int i = 0; i < numFiles; i++)
	{
		TouchController * touch = new TouchController(DISABLED); // set -1 to show blob is saved
		touch->setupParticles();
		touch->load(DIR.getName(i));
		
		touches.push_back(touch);
	}
}

/* Update
 ___________________________________________________________ */

void TouchesController::update()
{	
	for (int i = 0; i < touches.size(); i++) 
	{
		touches[i]->update();
	}	
}

/* Draw
 ___________________________________________________________ */

void TouchesController::draw()
{
	for(int i = 0; i < touches.size(); i++)
	{
		touches[i]->draw();
	}
}

/* Find closest touch
 ___________________________________________________________ */

void TouchesController::findClosest(int index)
{
	compare1.set(touches[index]->getModel()->getCurPos());
	
	for(int i = 0; i < touches.size(); i++)
	{
		compare2.set(touches[i]->getModel()->getStartPos());
		
		if(abs((int) compare1.distance(compare2)) < PROXIMITY) 
		{
			touches[i]->play();
			touches[index]->getModel()->hasPlaying = i;
			//hideAllBut(i);
		}
	}
}

/* Touch Events
 ___________________________________________________________ */


void TouchesController::touchStarted(int blobid, vector <ofPoint> pts, ofPoint centroid)
{
	if(numDrawing < SIM_ALLOWED_TOUCHES)
	{
		printf("Adding touch \n");
		
		TouchController * touch = new TouchController(blobid);
		touch->setupParticles();
		touch->setDateTime();
		touch->getModel()->drawing = true;
		touch->addPathPointAndScale(centroid.x, centroid.y, outlineScale);
		touch->setOutlineAndScale(pts, outlineScale);
		
		// jsut to test
		//touch->getModel()->visible = true;
		
		touches.push_back(touch);
		
		numDrawing++;
		
		//showAllBut(-1);
	}
}

void TouchesController::touchMoved(int blobid, vector <ofPoint> pts, ofPoint centroid)
{
	for(int i = 0; i < touches.size(); i++)
	{
		if (touches[i]->getModel()->blobid == blobid) 
		{
			touches[i]->addPathPointAndScale(centroid.x, centroid.y, outlineScale);
			touches[i]->setOutlineAndScale(pts, outlineScale);

			if (touches[i]->getModel()->hasPlaying == DISABLED) 
			{
				//findClosest(i);
			}

			break;
		}
	}
}

void TouchesController::touchEnded(int blobid)
{
	for(int i = 0; i < touches.size(); i++)
	{
		if (touches[i]->getModel()->blobid == blobid) 
		{
			printf("Removing touch \n");
			
			touches[i]->reset();
			//touches[i]->save();
			
			if(touches[i]->getModel()->hasPlaying != DISABLED)
			{
				touches[touches[i]->getModel()->hasPlaying]->reset();
			}
			
			numDrawing--;
			
			//hideAllBut(-1);
			
			touches[i]->getModel()->drawing = false;
			
			break;
		}
	}
}

/* Show / Hide all
 ___________________________________________________________ */

void TouchesController::showAllBut(int leaveOut, bool hideDrawing)
{
	for(int i = 0; i < touches.size(); i++)
	{
		if(i != leaveOut)
		{
			if(!hideDrawing)
			{
				touches[i]->show();
			}
			else if(hideDrawing && !touches[i]->getModel()->drawing)
			{
				touches[i]->show();
			}
		}
	}
}

void TouchesController::hideAllBut(int leaveOut, bool hideDrawing)
{
	for(int i = 0; i < touches.size(); i++)
	{
		if(i != leaveOut)
		{
			if(!hideDrawing)
			{
				touches[i]->hide();
			}
			else if(hideDrawing && !touches[i]->getModel()->drawing)
			{
				touches[i]->hide();
			}
		}
	}
}



