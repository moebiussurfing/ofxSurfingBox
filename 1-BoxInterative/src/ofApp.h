#pragma once
#include "ofMain.h"

#include "ofxSurfingBoxInteractive.h"

#include "ofxSurfingBoxHelpText.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void draw();
	void keyPressed(int key);
	
	//--

	// Box
	ofxSurfingBoxInteractive boxWidget;

	// Scene
	vector<ofColor> colors;

	//--

	// Just to show the help info
	ofxSurfingBoxHelpText textBoxWidget;
	void buildHelpInfo();
};
