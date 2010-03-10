#include "TouchesController.h"

/* Constructor
 ___________________________________________________________ */

TouchesController::TouchesController()
{
	_drawing = DISABLED;
	_playing = DISABLED;
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
	compare1.set(mouseX, mouseY);
	bool found = false;
	
	for (int i = 0; i < touches.size(); i++) 
	{
		if (i == _drawing) 
		{
			touches[i]->update(compare1.x, compare1.y);
		}
		else 
		{
			touches[i]->update();
			
			if(_playing == DISABLED)
			{
				if (_drawing != DISABLED) 
				{
					if(!found)
					{
						compare2.set(touches[i]->getCenterX(), touches[i]->getCenterY());
						
						if(abs((int) compare1.distance(compare2)) < PROXIMITY) 
						{
							touches[i]->play();
							_playing = i;
							found = true;
							hideAllBut(i);
						}
					}
				}
			}
			else if(i == _playing)
			{
				if(touches[_playing]->checkStopped())
				{
					touches[_playing]->reset();
					
					_playing = DISABLED;
				}
			}
		}
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

/* Touch Events
 ___________________________________________________________ */

void TouchesController::touchStarted(int blobid)
{
	// create new touch and start tracking
	TouchController * touch = new TouchController(blobid);
	touch->setupParticles();
	touch->setDateTime();
	touch->setTestOutline();
	
	touches.push_back(touch);
	
	_drawing = touches.size() - 1;
	
	showAllBut(-1);
}

void TouchesController::touchEnded()
{
	touches[_drawing]->reset();
	
	// save drawing to new file here
	//touches[_drawing]->save();
	
	if(_playing != DISABLED)
	{
		touches[_playing]->reset();
	}
	
	_playing = DISABLED;
	_drawing = DISABLED;
	
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
			else if(hideDrawing && i != _drawing)
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
		if(i != leaveOut && i != _drawing)
		{
			if(!hideDrawing)
			{
				touches[i]->hide();
			}
			else if(hideDrawing && i != _drawing)
			{
				touches[i]->hide();
			}
		}
	}
}



