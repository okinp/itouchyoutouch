#pragma once

class PVector {
public:
	float x, y, z;
	PVector() {
		x=0.0; y=0.0; z = 0.0;
	}
	PVector(float xPos, float yPos)
	{
		x = xPos; y = yPos; z = 0.0;
	}
};

class PColor {
public:
	float r, g, b, a;
	PColor() {
		r=0.0; g=1.0; b=1.0; a=1.0;
	}
};

class PTexture {
public:
	float u, v;
	PTexture() {
		u = 0.0; v = 0.0;
	}
};

class PTimer {
public:
	int state; 
	float time, duration;
	PTimer() {
		time = 0.0; duration = 0.0; state = 0;
	}
	
	float getPercent()
	{
		if (time > duration) 
		{
			time = duration;
			state = 0;
		}
		else if(time < 0)
		{
			time = 0;
			state = 0;
		}
		
		return time / duration;
	}
};

class PSetting 
{
public:
	
	
	float sizeMin;
	float sizeMax;
	float percent;
	float dirMin;
	float dirMax;
	float damping;
	float lifeMin;
	float lifeMax;
	float lifeSubMin;
	float lifeSubMax;
	
	PSetting() 
	{
		clear();
	}
	
	void clear()
	{
		sizeMin = 2.0;
		sizeMax = 8.0;
		percent = 1;
		dirMin = 0.2;
		dirMax = 1;
		damping = 0.2;
		lifeMin = 0.3;
		lifeMax = 1.0;
		lifeSubMin = 0.001;
		lifeSubMax = 0.004;
	}
};