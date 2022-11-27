#pragma once

#include "ofMain.h"

#include "ofxSurfingBoxInteractive.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void draw();
	void drawScene();
	void keyPressed(int key);
	
	// Box
	ofxSurfingBoxInteractive boxWidget;

	// Scene
	vector<ofColor> colors;
	ofColor c;
};
