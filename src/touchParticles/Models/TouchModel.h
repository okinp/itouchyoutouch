#pragma once

#include "Tools.h"
#include "ofxVec2f.h"
#include "Constants.h"

class TouchModel
{
public:

	int blobid;

	PVector center;
	
	vector <ofPoint> path;
	vector <ofPoint> outline;
	vector <PSetting> settings;
	int dateTime[6]; // 2010-3-4-15-36-7
	string fileName;

	bool visible;
	
	int hasPlaying;
	int playHead;
	bool playing;
	bool drawing;

	TouchModel()
	{
		hasPlaying = DISABLED;
		visible = false;
		drawing = false;
		
		PSetting setting1;
		setting1.percent = 0.80;
		setting1.dirMin = -0.2;
		setting1.dirMax = -0.9;
		setting1.lifeMin = 0.3;
		setting1.lifeMax = 0.6;
		settings.push_back(setting1);
		
		PSetting setting2;
		setting2.percent = 0.20;
		setting2.dirMin = -0.002;
		setting2.dirMax = -0.007;
		setting2.lifeMin = 0.1;
		setting2.lifeMax = 0.2;
		settings.push_back(setting2);
	}

	// Location Getters

	float getCenterX()
	{
		return path[playHead].x + center.x;
	}
	
	float getCenterY()
	{
		return path[playHead].y + center.y;
	}
	
	float getX()
	{
		return path[playHead].x;
	}
	
	float getY()
	{
		return path[playHead].y;
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