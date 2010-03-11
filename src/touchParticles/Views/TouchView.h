#pragma once

#include "ofMain.h"
#include "ofxVec2f.h"
#include "Tools.h"
#include "TouchModel.h"
#include "Constants.h"

class TouchView 
{
	
public:
	
	TouchView();
	
	void setModel(TouchModel * newModel);
	void setTexture(ofImage newTexture, int cellsInRow, int cellsInCol);
	
	void init();
	void update();
	void render();
	
	int getTotal();
	
	ofImage		texture;
	ofImage		flare;
	TouchModel * model;
	
private:
	
	void spawn(int i);

	void setParticleColor(int i, float r, float g, float b, float a=0.0);
	void setParticlePos(int i, float px, float py, float pz=0.0);
	void setParticleSize(int i, float particleDim);
	void addPosition(int i, float x, float y, float z);
	void setParticleTexCoords(int i, float columnID, float rowID);

	int			cellRows;
	int			cellColls;
	float		texW, texH;
	
	GLuint		particleVBO[3];
	
	float		dim[MAX_PARTICLES];			// particle size (w/h)
	PVector		pos[MAX_PARTICLES*4];		// vertex (quad) of particle
	PTexture	texcords[MAX_PARTICLES*4];	// texture coords
	PColor		color[MAX_PARTICLES*4];		// particle color rgba
	
	float		vel[MAX_PARTICLES][3];		
	float		acc[MAX_PARTICLES][3];
	float		damping[MAX_PARTICLES];
	
	float		life[MAX_PARTICLES][2];		//	[life] [life rate] 
	
	int numParticles;
};