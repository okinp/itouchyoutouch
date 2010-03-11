#pragma once

#include "Tools.h"
#include "ofxVec2f.h"
#include "Constants.h"

class TouchModel
{
public:

	int blobid;
	
	vector <ofPoint> path;
	vector <ofPoint> outline;
	vector <ofPoint> startOutline;
	vector <PSetting> settings;
	int dateTime[6]; // 2010-3-4-15-36-7
	string fileName;

	bool visible;
	
	int hasBond;
	int hasPlaying;
	int hasDrawing;
	int playHead;
	bool playing;
	bool drawing;
	
	TouchModel * drawingModel;
	TouchModel * playingModel;

	TouchModel()
	{
		hasBond = DISABLED;
		hasPlaying = DISABLED;
		hasDrawing = DISABLED;
		visible = false;
		drawing = false;
		
		moveAway();
	}
	
	void moveAway()
	{
		settings.clear();
		
		PSetting setting1;
		setting1.percent = 0.4;
		setting1.sizeMin = 5;
		setting1.sizeMax = 10;
		setting1.dirMin = 0.5;
		setting1.dirMax = 1;
		//setting1.dirMin = 0.2;
		//setting1.dirMax = 0.9;
		setting1.lifeMin = 0.15;
		setting1.lifeMax = 0.25;
		settings.push_back(setting1);
		
		PSetting setting2;
		setting2.percent = 0.6;
		setting1.sizeMin = 5;
		setting1.sizeMax = 10;
		setting2.dirMin = 2;
		setting2.dirMax = 3;
		setting2.lifeMin = 0.15;
		setting2.lifeMax = 0.25;
		settings.push_back(setting2);
	}

	// Location Getters

	ofPoint getCurPos()
	{
		if(playHead == 30)
		{
			startOutline = outline;
		}
		
		return path[playHead];
	}
	
	ofPoint getStartPos()
	{
		return path[0];
	}

	// Time getters
	
	int getYear()
	{
		return dateTime[0];
	}
	
	int getMonth()
	{
		return dateTime[1];
	}
	
	int getDay()
	{
		return dateTime[2];
	}
	
	int getHours()
	{
		return dateTime[3];
	}
	
	int getMinutes()
	{
		return dateTime[4];
	}
	
	int getSeconds()
	{
		return dateTime[5];
	}
};