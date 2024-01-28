#pragma once
#include "ofMain.h"

//-> We are testing this one here:
#include "ofxSurfingBoxInteractive.h"

// Only for help info. Not the purpose of this example.
#include "ofxSurfingBoxHelpText.h"

#include "surfingSceneTesters.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void draw();
	void drawScene();
	void keyPressed(int key);
	
	surfingSceneTesters scene;

	//--

	// Box
	ofxSurfingBoxInteractive boxWidget;

	// Scene
	vector<ofColor> colors;
	
	ofColor c;

	//--

	// Just to show the help info
	ofxSurfingBoxHelpText textBoxWidget;
	void buildHelpInfo();
};
