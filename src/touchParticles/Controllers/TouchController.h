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
	
	void init();
	
	void update();
	void updateCommon();
	void draw();
	
	void play();
	void reset();
	void hide();
	void show();
	
	bool checkStopped();
	
	void setTexture(ofImage newTexture, int cellsInRow, int cellsInCol);
	void setDateTime();
	void addPathPointAndScale(int xPos, int yPos, float scale);
	void setOutlineAndScale(vector <ofPoint> newOutline, float scale);
	void saveOutline();
	
	bool isAllowed();
	
	void setupParticles();
	
	TouchModel * getModel();
	
private:
	
	TouchView view;
	TouchModel * model;
	ofxXmlSettings _xml;
};