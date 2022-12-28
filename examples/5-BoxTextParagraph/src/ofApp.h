#pragma once

#include "ofMain.h"

//#define NANOVG_GL2_IMPLEMENTATION

#include "ofxSurfingBoxParagraph.h"


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
	TAB LEFT/RIGHT : Browse Layout Modes
	A : Lock Aspect Ratio
	B : Toggle Border
	K : Toggle Border Blink

*/


class ofApp : public ofBaseApp 
{
public:

	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	void drawSceneBoxed();

	ofxSurfingBoxParagraph boxWidget;

	std::string helpInfo = "";
	void buildHelpInfo();

	bool bModeSimple = false; // simple text or allow formating
};
