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
	// Load textures
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
			//else if(touches[hasPlaying]->checkStopped())
			//{
				//connectionStopped(i, hasPlaying);
			//}
			
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

void TouchesController::findClosest(int index)
{
	compare1.set(touches[index]->getModel()->getCurPos());
	
	for(int i = 0; i < touches.size(); i++)
	{
		compare2.set(touches[i]->getModel()->getStartPos());
		
		if(abs((int) compare1.distance(compare2)) < PROXIMITY && i != index && touches[i]->isAllowed()) 
		{
			connectionMade(index, i);
		}
	}
}

void TouchesController::showFurthestAway(int index)
{	
	int furthest = DISABLED;
	float distance = 0;
	
	compare1.set(touches[index]->getModel()->getCurPos());
	
	for(int i = 0; i < touches.size(); i++) 
	{
		compare2.set(touches[i]->getModel()->getCurPos());
		
		float diff = abs((int) compare1.distance(compare2));
			
		if(diff > distance && !touches[i]->getModel()->visible)
		{
			furthest = i;
			distance = diff;
		}
	}
	
	if(furthest != DISABLED)
	{
		touches[index]->getModel()->hasBond = furthest;
		touches[furthest]->show();
	}
}

/* Connections
 ___________________________________________________________ */

void TouchesController::connectionMade(int drawing, int playing)
{
	int textureNum = getNextTexture();
	touches[playing]->setTexture(textures[textureNum], NUM_ROWS, NUM_COLS);
	touches[drawing]->setTexture(textures[textureNum], NUM_ROWS, NUM_COLS);
	
	touches[playing]->play();
	touches[playing]->getModel()->hasDrawing = drawing;
	touches[playing]->getModel()->drawingModel = touches[drawing]->getModel();
	
	touches[drawing]->getModel()->hasPlaying = playing;
	touches[drawing]->getModel()->playingModel = touches[playing]->getModel();;
}

void TouchesController::connectionStopped(int drawing, int playing)
{
	touches[playing]->setTexture(textures[0], NUM_ROWS, NUM_COLS);
	touches[drawing]->setTexture(textures[0], NUM_ROWS, NUM_COLS);
	
	touches[playing]->reset();
	touches[drawing]->reset();
	
	// if touch created bond with other touch than the one it's playing with, and the bond is de-active, hide it
	if(touches[drawing]->getModel()->hasBond != DISABLED && touches[touches[drawing]->getModel()->hasBond]->isAllowed())
	{
		touches[touches[drawing]->getModel()->hasBond]->reset();
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
		
		showFurthestAway(touches.size() - 1);
		
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
				connectionStopped(i, touches[i]->getModel()->hasPlaying);
			}
			else if(touches[i]->getModel()->hasBond != DISABLED && touches[touches[i]->getModel()->hasBond]->isAllowed())
			{
				touches[touches[i]->getModel()->hasBond]->reset();
				touches[i]->reset();
			}
			else 
			{
				touches[i]->reset();
			}
			
			if(ENABLE_SAVE)
			{
				touches[i]->save();
			}
			
			numDrawing--;
			
			break;
		}
	}
}

/* Next Texture
 ___________________________________________________________ */

int TouchesController::getNextTexture()
{
	curTexture++;
	
	if(curTexture > NUM_TEXTURES - 1)
	{
		curTexture = 1;
	}
	
	return curTexture;
}

void TouchesController::reset()
{
	touches.clear();
}



