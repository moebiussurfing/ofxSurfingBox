#pragma once

#include "ofMain.h"

#include "ofxSurfingBoxInteractive.h"

/*

	HOW TO

	Use mouse to move the box.
	Double click to edit.
	Drag borders to change the scale.
	Use wheel to modify the scale +
	control (from center), 
	shift (width only) and 
	alt (height only) modifiers.
	Use mouse wheel too.

*/

class ofApp : public ofBaseApp 
{
public:

	void setup();
	void draw();
	void drawScene();
	void keyPressed(int key);
	
	ofxSurfingBoxInteractive boxWidget;
};
