#pragma once
#include "ofMain.h"

#include "ofxSurfingBoxHelpText.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void draw();
		void keyPressed(int key);
		
		//  We could update our info depending of our App Mode during runtime.
		void buildHelpInfo(); 

		ofxSurfingBoxHelpText textBoxWidget;
};
