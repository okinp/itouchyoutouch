#include "TouchView.h"

/* Constructor
_______________________________________________________________________ */

TouchView::TouchView() 
{
}

/* Init
 _______________________________________________________________________ */

void TouchView::init() 
{			
	numParticles = 0;

	// Setup the VBO
	
	glGenBuffersARB(3, &particleVBO[0]);	// Create 3 vertex buffer object. Give address to GLUint array where ID's are stored
	
	// color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[0]);	// Init VBO 0 as a vertex array VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*4*sizeof(float), color, GL_STREAM_DRAW_ARB);	// Copy data into VBO 0: 
	// Second parameter is the number of bytes to allocate
	// Third parameter is pointer to the actual data
	// Fourth parameter is for performance and says how the VBO is going to be used: Stream means that it will be changed every frame. Draw means data is sent to GPU
	
	// vertices
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*3*sizeof(float), pos, GL_STREAM_DRAW_ARB);
	
	// texture coords
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[2]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*2*sizeof(float), texcords, GL_STREAM_DRAW_ARB);
}

/* Update
 _______________________________________________________________________ */

void TouchView::update() 
{	
	if(model->visible)	
	{
		if(numParticles < MAX_PARTICLES)
		{
			for(int i = 0; i < 100; i++)
			{
				spawn(numParticles);
				
				numParticles++;
				
				if(numParticles >= MAX_PARTICLES)
					break;
			}
		}
	}
	
	for(int i=0; i<numParticles; i++) 
	{
		vel[i][0] += acc[i][0];
		vel[i][1] += acc[i][1];
		//vel[i][2] += acc[i][2];
		
		addPosition(i, vel[i][0], vel[i][1], vel[i][2]);
		
		vel[i][0] *= damping[i];
		vel[i][1] *= damping[i];
		vel[i][2] *= damping[i];
		
		// fade by the life rate
		life[i][0] -= life[i][1];
		
		setParticleColor(i, 1.0, 1.0, 1.0, life[i][0]);
		
		if(life[i][0] <= 0.0) 
		{
			if(model->visible)	
			{
				spawn(i);
			}
		}
	}
}

/* Create / Reset Particle
 _______________________________________________________________________ */

void TouchView::spawn(int i) 
{
	float percent = 0.0;
	float random = ofRandomuf();
	
	//printf("Setting size: %zd \n", settings.size());
	
	for (int j = 0; j < model->settings.size(); j++) 
	{
		percent += model->settings[j].percent;
		
		if (random < percent) 
		{
			setParticleSize(i, ofRandom(model->settings[j].sizeMin, model->settings[j].sizeMax));
			setParticleColor(i, 1, 1, 1, 1);
			
			int max = model->outline.size() - 1;
			PVector thePoint = model->outline[ofRandom(0, max)];
			
			// set static center for test 
			PVector center;
			center.x = 150;
			center.y = 200;
			
			ofxVec2f direction; 
			direction.x = thePoint.x - center.x;
			direction.y = thePoint.y - center.y;
			direction.normalize();
			
			setParticlePos(i, thePoint.x + model->getX(), thePoint.y + model->getY());
			setParticleTexCoords(i, (int)ofRandom(0, 2), (int)ofRandom(0, 2));
			
			direction *= ofRandom(model->settings[j].dirMin, model->settings[j].dirMax);
			
			life[i][0] = ofRandom(model->settings[j].lifeMin, model->settings[j].lifeMax);
			life[i][1] = ofRandom(model->settings[j].lifeSubMin, model->settings[j].lifeSubMax);
			
			acc[i][0] = direction.x;
			acc[i][1] = direction.y;
			acc[i][2] = 0;
			
			damping[i] = model->settings[j].damping;
			
			break;
		}
	}	
}


/* Render
 _______________________________________________________________________ */

void TouchView::render() 
{			
	glEnable(GL_TEXTURE_2D);	// Tells OpenGL that we want ot draw a 2d teture
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); // Enables the Texture coordinate array for drawing with glDrawArrays or glDrawElements calls 
	glEnableClientState(GL_VERTEX_ARRAY); // Enables the Vertex array for drawing with glDrawArrays or glDrawElements calls 
	glEnableClientState(GL_COLOR_ARRAY); // Enables the Color array for drawing with glDrawArrays or glDrawElements calls 
	
	// bind tex coors
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[2]); // init VBO (see above)
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*2*sizeof(float), texcords); // put data in VBO
	glTexCoordPointer(2, GL_FLOAT, 0, 0); // Tell OpenGL that we have 2 coordinates and the coordinates in the array are floats
	
	
	// bind color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[0]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*4*sizeof(float), color);
	glColorPointer(4, GL_FLOAT, 0, 0);
	
	
	// bind vertices [these are quads]
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[1]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*3*sizeof(float), pos);
	glVertexPointer(3, GL_FLOAT, 0, 0); // Tell OpenGL that we have 3 coordinates (x, y, z) and the coordinates are stored as floats in the array
	
	// draw the vbo
	glDisable(GL_DEPTH_TEST); // disable depth test (whatever it is)
	ofEnableArbTex();	
	
	ofEnableAlphaBlending();
	texture.getTextureReference().bind();
	
	glDrawArrays(GL_QUADS, 0, MAX_PARTICLES*4);  // render all array data we putted in VBO's as quads, at index 0, and to the end of the arrays
	
	texture.getTextureReference().unbind();
	ofDisableAlphaBlending();
	
	ofDisableArbTex();
	glEnable(GL_DEPTH_TEST);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}



/* Set Particle Texture Coordinates
 _______________________________________________________________________ */

void TouchView::setParticleTexCoords(int i, float columnID, float rowID) 
{	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	if(columnID > cellColls) columnID = cellColls;
	if(rowID    > cellRows)	 rowID	  = cellRows;
	
	if(columnID < 0) columnID = 0;
	if(rowID < 0)	 rowID	  = 0;
	
	// get the cell image width
	float cellWidth  = texW / cellRows;
	float cellHeight = texH / cellColls;
	
	float row = rowID;
	float col = columnID;
	
	// P1
	texcords[(i*4)+0].u = (cellWidth * row)		/ texW;
	texcords[(i*4)+0].v = (cellHeight * col)	/ texH;
	
	// P2
	texcords[(i*4)+1].u = ((cellWidth * row)	+ cellWidth)	/ texW;
	texcords[(i*4)+1].v = (cellHeight * col)	/ texH;
	
	// P2
	texcords[(i*4)+2].u = ((cellWidth * row) + cellWidth)		/ texW;
	texcords[(i*4)+2].v = ((cellHeight * col) + cellHeight)	/ texH;	
	
	// P2
	texcords[(i*4)+3].u = (cellWidth * row)		/ texW;
	texcords[(i*4)+3].v = ((cellHeight * col)+cellHeight)	/ texH;	
}

/* Set Particle Color
_______________________________________________________________________ */

void TouchView::setParticleColor(int i, float r, float g, float b, float a) 
{
	if(i < 0) i = 0;
	if(i > MAX_PARTICLES) i = MAX_PARTICLES;
	
	
	// Color 1
	color[(i*4)+0].r = r;
	color[(i*4)+0].g = g;
	color[(i*4)+0].b = b;
	color[(i*4)+0].a = a;
	
	// Color 2
	color[(i*4)+1].r = r;
	color[(i*4)+1].g = g;
	color[(i*4)+1].b = b;
	color[(i*4)+1].a = a;
	
	// Color 3
	color[(i*4)+2].r = r;
	color[(i*4)+2].g = g;
	color[(i*4)+2].b = b;
	color[(i*4)+2].a = a;
	
	// Color 4
	color[(i*4)+3].r = r;
	color[(i*4)+3].g = g;
	color[(i*4)+3].b = b;
	color[(i*4)+3].a = a;
	
}

/* Set Particle Position
 _______________________________________________________________________ */

void TouchView::setParticlePos(int i, float px, float py, float pz) 
{	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	// P1
	pos[(i*4)+0].x = px;
	pos[(i*4)+0].y = py;
	pos[(i*4)+0].z = pz;
	
	// P2
	pos[(i*4)+1].x = px + dim[i];
	pos[(i*4)+1].y = py;
	pos[(i*4)+1].z = pz;
	
	// P2
	pos[(i*4)+2].x = px + dim[i];
	pos[(i*4)+2].y = py + dim[i];
	pos[(i*4)+2].z = pz;
	
	// P2
	pos[(i*4)+3].x = px;
	pos[(i*4)+3].y = py + dim[i];
	pos[(i*4)+3].z = pz;
}

/* Set Particle Size
_______________________________________________________________________ */

void TouchView::setParticleSize(int i, float particleDim) 
{	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	dim[i] = particleDim;
}

/* Add Position
_______________________________________________________________________ */

void TouchView::addPosition(int i, float x, float y, float z) 
{	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	// P1
	pos[(i*4)+0].x += x;
	pos[(i*4)+0].y += y;
	pos[(i*4)+0].z += z;
	
	// P2
	pos[(i*4)+1].x += x;
	pos[(i*4)+1].y += y;
	pos[(i*4)+1].z += z;
	
	// P2
	pos[(i*4)+2].x += x;
	pos[(i*4)+2].y += y;
	pos[(i*4)+2].z += z;
	
	// P2
	pos[(i*4)+3].x += x;
	pos[(i*4)+3].y += y;
	pos[(i*4)+3].z += z;
}

/* Load texture
_______________________________________________________________________ */

void TouchView::loadTexture(string path, int cellsInRow, int cellsInCol) 
{
	ofDisableArbTex();
	texture.loadImage(path);
	ofEnableArbTex();
	
	texW = texture.getWidth();
	texH = texture.getHeight();
	
	cellRows  = cellsInRow;
	cellColls = cellsInCol; 
}

/* Getter / Setter
 _______________________________________________________________________ */

int TouchView::getTotal()
{
	return numParticles;
}

void TouchView::setModel(TouchModel * newModel)
{
	model = newModel;
}



