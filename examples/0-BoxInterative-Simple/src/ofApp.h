#pragma once

#include "ofMain.h"

#include "ofxSurfingBoxInteractive.h"

/*

	HOW TO

	> MOUSE
	Double click to Toggle Edit.
	Use Mouse Drag to move the box.
	Drag borders to change the scale.
	Use Mouse Wheel to modify the scale 
	from the left.
	+CONTROL (from center), 
	+SHIFT (width only) and 
	+ALT (height only) modifiers.

	> KEYS
	SPACE : Toggle Edit
	BACKSPACE : Reset
	TAB: Browse Layout Modes
	d : enable draw scene that uses the box

*/


class ofApp : public ofBaseApp 
{
public:

	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofxSurfingBoxInteractive boxWidget;
	void drawSceneBoxed();
	bool bDrawScene = 1;
};
