#pragma once

#include "ofMain.h"

#include "ofxSurfingBoxHelpText.h"

class ofApp : public ofBaseApp
{
	public:

		void setup();
		void draw();
		void keyPressed(int key);
		
		void buildHelpInfo(); 

		ofxSurfingBoxHelpText textBoxWidget;
};
