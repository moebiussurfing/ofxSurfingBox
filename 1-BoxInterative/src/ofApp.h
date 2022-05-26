#pragma once
#include "ofMain.h"

#include "ofxSurfingBoxInteractive.h"

#include "ofxSurfingBoxHelpText.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void draw();
	void keyPressed(int key);

	ofxSurfingBoxInteractive boxWidget;
	ofRectangle r;
	vector<ofColor> colors;

	//--

	ofxSurfingBoxHelpText textBoxWidget;
	void buildHelpInfo();
};
