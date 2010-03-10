#include "ofMain.h"
#include "ofxCvBlobTracker.h"
#include "ofxCvTrackedBlob.h"
#include "ofxCvConstants_Track.h"
#include "sensing.h"
#include "TouchesController.h"

class testApp : public ofSimpleApp, public ofxCvBlobListener 
{
  public:
    
	void setup();
    void update();
    void draw();

    void keyPressed( int key );
    void mouseMoved( int x, int y );
    void mouseDragged( int x, int y, int button );
    void mousePressed( int x, int y, int button );
    void mouseReleased();

    void blobOn( int x, int y, int id, int order );
    void blobMoved( int x, int y, int id, int order );
    void blobOff( int x, int y, int id, int order );
	
	sensing * mySensing;
	TouchesController * touches;
};

