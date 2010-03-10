#include "ofMain.h"
/*  CV  Headers */
#include "ofxCvBlobTracker.h"
#include "ofxCvTrackedBlob.h"
#include "ofxCvConstants_Track.h"
#include "sensing.h"

class testApp : public ofSimpleApp, public ofxCvBlobListener {

  public:
	sensing* mySensing;
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

};

