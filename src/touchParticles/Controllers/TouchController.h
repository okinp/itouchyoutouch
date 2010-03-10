#pragma once

#include "ofMain.h"
#include "Tools.h"
#include "ofxXmlSettings.h"
#include "TouchView.h"
#include "TouchModel.h"
#include "Constants.h"

class TouchController
{
	
public:
	
	TouchController(int blobid);
	
	void load(string fileName);
	void save();
	void setFileName(string fileName);
	void detect(string imgPath);
	
	void loadXML();
	void loadParticles();
	
	void addPathPoint(int xPos, int yPos);
	void update();
	void updateCommon();
	void draw();
	
	void play();
	void reset();
	void hide();
	void show();
	
	bool checkStopped();
	
	void setDateTime();
	void setOutline(vector <ofPoint> newOutline);
	void setTestOutline();
	
	
	void setupParticles();
	
	float getCenterX();
	float getCenterY();
	
	TouchModel * getModel();
	
private:
	
	TouchView view;
	TouchModel * model;
	ofxXmlSettings _xml;
};