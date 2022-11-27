#pragma once

#include "ofMain.h"

#include "ofxSurfingBoxInteractive.h"

/*

	HOW TO
	Use mouse to move the box.
	Drag borders to change the scale.
	Use wheel to modify the scale
	Use control, shift and alt modifiers.
	Use mouse wheel too.

*/

class ofApp : public ofBaseApp 
{

public:

	void setup();
	void draw();
	void drawScene();
	void keyPressed(int key);
	
	// Box
	ofxSurfingBoxInteractive boxWidget;
};
