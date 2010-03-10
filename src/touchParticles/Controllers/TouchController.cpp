#include "TouchController.h"

/* Constructor
 ___________________________________________________________ */

TouchController::TouchController(int blobid)
{
	model = new TouchModel();
	
	model->blobid = blobid;
	model->fileName = "";
	model->playHead = 0;
	model->playing = false;
	
	view.setModel(model);
}

void TouchController::setupParticles()
{
	view.loadTexture("particleGrid2.png", 2, 2);
	view.init();
}

/* Update : Call on current touch
 ___________________________________________________________ */

void TouchController::addPathPoint(int xPos, int yPos)
{		
	model->path.push_back(ofPoint(xPos, yPos));
	model->playHead = model->path.size() - 1;
	
	updateCommon();
}

/* Update : Call on all other
 ___________________________________________________________ */

void TouchController::update()
{	
	if(model->playing)
	{
		model->playHead++;
		
		// stop at last frame
		if(model->playHead > model->path.size() - 1)
		{
			model->playHead = model->path.size() - 1;
		}
	}
	
	updateCommon();
}

/* Update : Common update for all
 ___________________________________________________________ */

void TouchController::updateCommon()
{		
	view.update();
}

/* Draw
 ___________________________________________________________ */


void TouchController::draw()
{	
	view.render();
	
	if(model->playing)
	{
		ofSetColor(0, 255, 0);
		ofCircle(model->getCenterX(), model->getCenterY(), 5);
	}
}


/* Play : Call on selected old touch to interact
 ___________________________________________________________ */

void TouchController::play()
{	
	model->playing = true;
}

void TouchController::reset()
{
	model->playing = false;
	model->playHead = 0;
	model->blobid = -1;
}

bool TouchController::checkStopped()
{	
	if (model->playing && model->playHead == model->path.size() - 1) 
	{		
		return true;
	}
	
	return false;
}

/* Hide / show
 ___________________________________________________________ */

void TouchController::show()
{
	model->visible = true;
}

void TouchController::hide()
{
	model->visible = false;
}

/* Load / Save
 ___________________________________________________________ */

void TouchController::load(string fileName)
{	
	model->fileName = fileName;
	model->path.clear();
	model->outline.clear();
	
	loadXML();
}

void TouchController::loadXML()
{
	if(_xml.loadFile(FOLDER + model->fileName) )
	{
		if(_xml.pushTag(TOUCH, 0))
		{
			// center
			model->center.x = (float) _xml.getAttribute(CENTER, Y, 0.0, 0);
			model->center.y = (float) _xml.getAttribute(CENTER, Y, 0.0, 0);
			
			// Outline
			if(_xml.pushTag(OUTLINE, 0))
			{
				for (int i = 0; i < _xml.getNumTags(POINT); i++) 
				{
					ofPoint point;
					point.x = (float) _xml.getAttribute(POINT, X, 0, i);
					point.y = (float) _xml.getAttribute(POINT, Y, 0, i);
					model->outline.push_back(point);
				}
				
				_xml.popTag();
			}
			
			// Path
			if(_xml.pushTag(PATH, 0))
			{
				for (int i = 0; i < _xml.getNumTags(POINT); i++) 
				{
					ofPoint point;
					point.x = (float) _xml.getAttribute(POINT, X, 0, i);
					point.y = (float) _xml.getAttribute(POINT, Y, 0, i);
					model->path.push_back(point);
				}
				
				_xml.popTag();
			}
			
			// Datetime
			model->dateTime[0] = (int) _xml.getAttribute(DATETIME, YEAR, 0, 0);
			model->dateTime[1] = (int) _xml.getAttribute(DATETIME, MONTH, 0, 0);
			model->dateTime[2] = (int) _xml.getAttribute(DATETIME, DAY, 0, 0);
			model->dateTime[3] = (int) _xml.getAttribute(DATETIME, HOUR, 0, 0);
			model->dateTime[4] = (int) _xml.getAttribute(DATETIME, MINUTE, 0, 0);
			model->dateTime[5] = (int) _xml.getAttribute(DATETIME, SECOND, 0, 0);
			
			_xml.popTag();
		}
		
		//printf("Outside Size: %d \n", model->outline.size());
		view.setModel(model);
	}
	
}

void TouchController::save()
{
	_xml.loadFromBuffer("<root></root>");
	
	_xml.clear();
	
	_xml.addTag(TOUCH);
	_xml.pushTag(TOUCH, 0);
	
	_xml.addTag(CENTER);
	
	_xml.addAttribute(CENTER, X, ofToString(model->center.x, 1), 0);
	_xml.addAttribute(CENTER, Y, ofToString(model->center.y, 1), 0);
	
	_xml.addTag(OUTLINE);
	_xml.pushTag(OUTLINE, 0);
	for(int i = 0; i < model->outline.size(); i++)
	{
		_xml.addTag(POINT);
		_xml.addAttribute(POINT, X, ofToString(model->outline[i].x, 1), i);
		_xml.addAttribute(POINT, Y, ofToString(model->outline[i].y, 1), i);
	}
	_xml.popTag();
	
	_xml.addTag(PATH);
	_xml.pushTag(PATH, 0);
	for(int i = 0; i < model->path.size(); i++)
	{
		_xml.addTag(POINT);
		_xml.addAttribute(POINT, X, ofToString(model->path[i].x, 1), i);
		_xml.addAttribute(POINT, Y, ofToString(model->path[i].y, 1), i);
	}
	_xml.popTag();
	
	_xml.addTag(DATETIME);
	_xml.addAttribute(DATETIME, YEAR, ofToString(model->dateTime[0], 0), 0);
	_xml.addAttribute(DATETIME, MONTH, ofToString(model->dateTime[1], 0), 0);
	_xml.addAttribute(DATETIME, DAY, ofToString(model->dateTime[2], 0), 0);
	_xml.addAttribute(DATETIME, HOUR, ofToString(model->dateTime[3], 0), 0);
	_xml.addAttribute(DATETIME, MINUTE, ofToString(model->dateTime[4], 0), 0);
	_xml.addAttribute(DATETIME, SECOND, ofToString(model->dateTime[5], 0), 0);
	
	string file = ofToString(model->dateTime[0]);
	file += "-";
	file += ofToString(model->dateTime[1]);
	file += "-";
	file += ofToString(model->dateTime[2]);
	file += "-";
	file += ofToString(model->dateTime[3]);
	file += "-";
	file += ofToString(model->dateTime[4]);
	file += "-";
	file += ofToString(model->dateTime[5]);
	
	if(model->fileName != "")
	{
		_xml.saveFile(FOLDER + model->fileName);
	}
	else 
	{		
		// this may fuck up if 2 people touch within a second
		_xml.saveFile(FOLDER + file + ".xml");
	}
}

/* Setters
 ___________________________________________________________ */

void TouchController::setDateTime()
{
	model->dateTime[0] = ofGetYear();
	model->dateTime[1] = ofGetMonth();
	model->dateTime[2] = ofGetDay();
	model->dateTime[3] = ofGetHours();
	model->dateTime[4] = ofGetMinutes();
	model->dateTime[5] = ofGetSeconds();
}

void TouchController::setFileName(string fileName)
{
	model->fileName = fileName;
}

void TouchController::setOutline(vector <ofPoint> newOutline)
{
	model->outline.clear();
	model->outline = newOutline;
}

void TouchController::setTestOutline()
{
	// just a test outline/ for new touches
	ofPoint p1;
	model->outline.push_back(p1);
	p1.x += 30;
	model->outline.push_back(p1);
	p1.y += 30;
	model->outline.push_back(p1);
	p1.x -= 30;
	model->outline.push_back(p1);
	p1.y -= 30;
	model->outline.push_back(p1);
}

float TouchController::getCenterX()
{
	return model->getCenterX();
}

float TouchController::getCenterY()
{
	return model->getCenterY();
}

TouchModel * TouchController::getModel()
{
	return model;
}