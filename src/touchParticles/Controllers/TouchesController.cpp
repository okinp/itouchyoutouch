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

/* Distance methods
 ___________________________________________________________ */

void TouchesController::showNeighbours(int index)
{
	vector <int> found;
	found.push_back(index);
	
	printf("There are %d touches in total \n", touches.size());
	printf("Index is %d \n", index);

	
	for (int i = 0; i < touches.size(); i++) 
	{
		compare1.set(touches[i]->getModel()->getCurPos());
		
		bool foundLower = false;
		
		// compare to every found neighbour
		for(int j = 0; j < found.size(); j++)
		{
			compare2.set(touches[found[j]]->getModel()->getCurPos());
			
			// if lower than threshold, discard
			if(abs((int) compare1.distance(compare2)) < PROXIMITY_NEIGHBOUR)
			{
				printf("Touch %d with x: %f y: %f is lower than found %d with x: %f y: %f \n", i, touches[i]->getModel()->getCurPos().x, touches[i]->getModel()->getCurPos().y, found[j], touches[found[j]]->getModel()->getCurPos().x, touches[found[j]]->getModel()->getCurPos().y);
				
				foundLower = true;
			}
			else {
				printf("Touch %d with x: %f y: %f is higher than found %d with x: %f y: %f \n", i, touches[i]->getModel()->getCurPos().x, touches[i]->getModel()->getCurPos().y, j, touches[found[j]]->getModel()->getCurPos().x, touches[found[j]]->getModel()->getCurPos().y);
			}

		}
		
		if(!foundLower)
		{
			found.push_back(i);
			
			printf("Saved %d in found\n", i);
		}
	}
	
	printf("Found: %zd \n", found.size());
	
	for(int i = 0; i < found.size(); i++)
	{
		touches[found[i]]->show();
	}
}

void TouchesController::findClosest(int index)
{
	// THIS SHOULD ONLY WORK ON VISIBLE TOUCHES
	
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
		TouchController * touch = new TouchController(blobid);
		touch->setupParticles();
		touch->setDateTime();
		touch->getModel()->drawing = true;
		touch->addPathPointAndScale(centroid.x, centroid.y, outlineScale);
		touch->setOutlineAndScale(pts, outlineScale);
		touch->saveOutline(); // this sets the outline used for playback
		touch->show();
		
		printf("Touches size before pushing new touch: %zd \n", touches.size());
		
		touches.push_back(touch);
		
		showNeighbours(touches.size() - 1);
		
		numDrawing++;
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



