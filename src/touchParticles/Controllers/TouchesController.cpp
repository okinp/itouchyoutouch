#include "TouchesController.h"

/* Constructor
 ___________________________________________________________ */

TouchesController::TouchesController()
{
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

void TouchesController::update(float mouseX, float mouseY)
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
	compare1.set(touches[index]->getCenterX(), touches[index]->getCenterY());
	
	for(int i = 0; i < touches.size(); i++)
	{
		compare2.set(touches[i]->getCenterX(), touches[i]->getCenterY());
		
		if(abs((int) compare1.distance(compare2)) < PROXIMITY) 
		{
			touches[i]->play();
			touches[index]->getModel()->hasPlaying = i;
			hideAllBut(i);
		}
	}
}

/* Touch Events
 ___________________________________________________________ */


void TouchesController::touchStarted(ofxCvTrackedBlob blob)
{
	TouchController * touch = new TouchController(blob.id);
	touch->setupParticles();
	touch->setDateTime();
	touch->getModel()->drawing = true;
	touch->addPathPoint(blob.centroid.x, blob.centroid.y);
	touch->setOutline(blob.pts);
	
	touches.push_back(touch);
	
	showAllBut(-1);
}

void TouchesController::touchMoved(ofxCvTrackedBlob blob)
{
	for(int i = 0; i < touches.size(); i++)
	{
		if (touches[i]->getModel()->blobid == blob.id) 
		{
			touches[i]->addPathPoint(blob.centroid.x, blob.centroid.y);
			touches[i]->setOutline(blob.pts);

			if (touches[i]->getModel()->hasPlaying == DISABLED) 
			{
				findClosest(i);
			}

			break;
		}
	}
}

void TouchesController::touchEnded(ofxCvTrackedBlob blob)
{
	for(int i = 0; i < touches.size(); i++)
	{
		if (touches[i]->getModel()->blobid == blob.id) 
		{
			touches[i]->reset();
			//touches[i]->save();
			
			if(touches[i]->getModel()->hasPlaying != DISABLED)
			{
				touches[touches[i]->getModel()->hasPlaying]->reset();
			}
			
			break;
		}
	}
	
	hideAllBut(-1);
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



