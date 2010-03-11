#include "sensing.h"

sensing::sensing(ofxCvBlobListener * listener)
{
	disabled = true;
	cwidth = VIDEO_WIDTH;
    cheight = VIDEO_HEIGHT;
	threshold = 60;
	blurAmount = 11;
	area = 4500;
	bLearnBakground = true;
	show = false;
	
	vidGrabber.initGrabber( cwidth, cheight );
	colorImg.allocate( cwidth, cheight );
	grayImg.allocate( cwidth, cheight );
	bgImg.allocate( cwidth, cheight );
	outputTexture.allocate(cwidth, cheight, GL_RGB);
	blobTracker.setListener(listener);
	
	gui.addTitle("Input");
	gui.addContent("Input", outputTexture);
	gui.addSlider("Threshold", threshold , 0.0, 255);
	gui.addSlider("Bluring", blurAmount , 0, 40);
	gui.addContent("Difference", grayImg);
	gui.addSlider("Area",area, 10, 6000);
	gui.addToggle("Disabled", disabled);
	gui.show();

}
void sensing::update()
{
	vidGrabber.grabFrame();
	
	if( vidGrabber.isFrameNew() ) 
	{
        colorImg = vidGrabber.getPixels();
        colorImg.mirror( false, true );
        grayImg = colorImg;
		
        if( bLearnBakground ) 
		{
            bgImg = grayImg;
            bLearnBakground = false;
        }
		
        grayImg.absDiff( bgImg );
        grayImg.blur( blurAmount );
        grayImg.threshold( threshold );
		
        //findContures( img, minSize, maxSize, nMax, inner contours yes/no )
        contourFinder.findContours( grayImg, area, 300000, 20, false );
        blobTracker.trackBlobs( contourFinder.blobs );
    }
}

void sensing::draw()
{	
	if (show) 
	{
		gui.draw();
		
		outputTexture.begin();
		ofSetColor( 0xffffff );
		colorImg.draw( 0,0 );
		blobTracker.draw(0, 0);
		outputTexture.end();
		//grayImg.draw( 360,200 );
		ofDrawBitmapString( "Press B to learn background", 100, 510);
	}
}

