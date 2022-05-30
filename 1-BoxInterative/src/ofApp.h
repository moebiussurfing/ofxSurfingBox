#pragma once
#include "ofMain.h"

//-> We are testing this one here:
#include "ofxSurfingBoxInteractive.h"

// Only for help info. Not the purpose of this example.
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
