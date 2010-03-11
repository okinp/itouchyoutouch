#include "TouchesController.h"

/* Constructor
 ___________________________________________________________ */

TouchesController::TouchesController()
{
	numDrawing = 0;
	curTexture = 1;
	
	outlineScale = ofGetWidth() / VIDEO_WIDTH;
	float diff = (ofGetHeight() / VIDEO_HEIGHT) - outlineScale;
	
	if(diff != 0.0)
	{
		printf("ERROR: Width and height scale is wrong: %f \n", diff);
	}
}

/* Load
 ___________________________________________________________ */

void TouchesController::load()
{
	// Load textues
	
	for(int i = 0; i < NUM_TEXTURES; i++)
	{
		textures[i].loadImage("particleGrid" + ofToString(i, 0) + ".png");
	}
	
	
	// Load XML files
	
	ofxDirList DIR;
	DIR.allowExt("xml");
	
	int numFiles = DIR.listDir(FOLDER);
	
	for(int i = 0; i < numFiles; i++)
	{
		TouchController * touch = new TouchController(DISABLED); // set -1 to show blob is saved
		touch->setTexture(textures[0], NUM_ROWS, NUM_COLS);
		touch->init();
		touch->load(DIR.getName(i));
		
		touches.push_back(touch);
	}
}

/* Update
 ___________________________________________________________ */

void TouchesController::touchMoved(int blobid, vector <ofPoint> pts, ofPoint centroid)
{
	for(int i = 0; i < touches.size(); i++)
	{
		if (touches[i]->getModel()->blobid == blobid && touches[i]->getModel()->drawing) 
		{
			touches[i]->addPathPointAndScale(centroid.x, centroid.y, outlineScale);
			touches[i]->setOutlineAndScale(pts, outlineScale);
			
			int hasPlaying = touches[i]->getModel()->hasPlaying;
			
			// if no connection is made
			if (hasPlaying == DISABLED) 
			{
				findClosest(i);
			}
			// else if playing connection has stopped
			else if(touches[hasPlaying]->checkStopped())
			{
				touches[i]->getModel()->hasPlaying = DISABLED;
			}
			
			break;
		}
	}
}

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
				foundLower = true;
			}
		}
		
		if(!foundLower)
		{
			found.push_back(i);
		}
	}
	
	for(int i = 0; i < found.size(); i++)
	{
		touches[found[i]]->show();
	}
}

void TouchesController::findClosest(int index)
{
	compare1.set(touches[index]->getModel()->getCurPos());
	
	for(int i = 0; i < touches.size(); i++)
	{
		compare2.set(touches[i]->getModel()->getStartPos());
		
		if(abs((int) compare1.distance(compare2)) < PROXIMITY && i != index && touches[i]->isAllowed()) 
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
		//touch->setupParticles();
		touch->setTexture(textures[0], NUM_ROWS, NUM_COLS);
		touch->init();
		touch->setDateTime();
		touch->getModel()->drawing = true;
		touch->addPathPointAndScale(centroid.x, centroid.y, outlineScale);
		touch->setOutlineAndScale(pts, outlineScale);
		touch->saveOutline(); // this sets the outline used for playback
		touch->show();
		
		touches.push_back(touch);
		
		showNeighbours(touches.size() - 1);
		
		numDrawing++;
	}
}

void TouchesController::touchEnded(int blobid)
{
	for(int i = 0; i < touches.size(); i++)
	{
		if (touches[i]->getModel()->blobid == blobid) 
		{
			if(touches[i]->getModel()->hasPlaying != DISABLED)
			{
				touches[touches[i]->getModel()->hasPlaying]->reset();
			}
			
			touches[i]->reset();
			//touches[i]->save();
			
			numDrawing--;
			
			//hideAllBut(-1);
			
			break;
		}
	}
}

/* Next Texture
 ___________________________________________________________ */

int TouchesController::nextTexture()
{
	curTexture++;
	
	if(curTexture > NUM_TEXTURES - 1)
	{
		curTexture = 1;
	}
	
	return curTexture;
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



