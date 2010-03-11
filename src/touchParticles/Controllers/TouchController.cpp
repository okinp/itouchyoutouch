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

void TouchController::init()
{
	view.init();
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
	
	view.update();
}

/* Draw
 ___________________________________________________________ */


void TouchController::draw()
{	
	view.render();
	
	if(DEBUG)
	{
		if(model->playing)
		{
			ofSetColor(0, 255, 0);
			ofCircle(model->getCurPos().x, model->getCurPos().y, 5);
		}
		else if(model->drawing)
		{
			ofSetColor(255, 0, 0);
			ofCircle(model->getCurPos().x, model->getCurPos().y, 5);
		}
		else 
		{
			ofSetColor(255, 255, 255);
			ofCircle(model->getCurPos().x, model->getCurPos().y, 5);
		}
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
	model->outline = model->startOutline;
	model->playing = false;
	model->drawing = false;
	model->hasPlaying = DISABLED;
	model->hasDrawing = DISABLED;
	model->hasBond = DISABLED;
	model->drawingModel = NULL;
	model->playingModel = NULL;
	model->visible = false;
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

	_xml.addTag(OUTLINE);
	_xml.pushTag(OUTLINE, 0);
	for(int i = 0; i < model->startOutline.size(); i++)
	{
		_xml.addTag(POINT);
		_xml.addAttribute(POINT, X, ofToString(model->startOutline[i].x, 1), i);
		_xml.addAttribute(POINT, Y, ofToString(model->startOutline[i].y, 1), i);
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

void TouchController::addPathPointAndScale(int xPos, int yPos, float scale)
{		
	//xPos = xPos * scale;
	//yPos = yPos * scale;
	
	xPos = (xPos - 30) * 4.1;
	yPos = (yPos - 40) * 4.2;
	
	model->path.push_back(ofPoint(xPos, yPos));
	model->playHead = model->path.size() - 1;
}

void TouchController::setOutlineAndScale(vector <ofPoint> newOutline, float scale)
{
	model->outline.clear();
	
	for(int i = 0; i < newOutline.size(); i++)
	{
		//newOutline[i] *= scale;
		
		// calibration
		newOutline[i].x = (newOutline[i].x - 20) * 4.1;
		newOutline[i].y = (newOutline[i].y - 40) * 4.2;
	}
	
	model->outline = newOutline;
}

bool TouchController::isAllowed()
{
	if(model->playing || model->drawing || !model->visible)
	{
		return false;
	}
	
	return true;
}

void TouchController::saveOutline()
{
	model->startOutline = model->outline;
}

void TouchController::setDateTime()
{
	model->dateTime[0] = ofGetYear();
	model->dateTime[1] = ofGetMonth();
	model->dateTime[2] = ofGetDay();
	model->dateTime[3] = ofGetHours();
	model->dateTime[4] = ofGetMinutes();
	model->dateTime[5] = ofGetSeconds();
}

void TouchController::setTexture(ofImage newTexture, int cellsInRow, int cellsInCol) 
{
	view.setTexture(newTexture, cellsInRow, cellsInCol);
}

void TouchController::setFileName(string fileName)
{
	model->fileName = fileName;
}

TouchModel * TouchController::getModel()
{
	return model;
}