#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	//ofSetWindowPosition(-1920, 23);

	//--

	// Optional

	// Customize to avoid collide 
	// when using multiple instances!
	boxWidget.setName("Demo"); // for the filename
	boxWidget.setPath("MyAddon/"); // for the container folder

	//--

	boxWidget.setup();

	//--

	// Optional

	// Debug
	//boxWidget.setDebug(true);
	//boxWidget.setDebugDoubleClick(true);

	// Force edit on startup
	//boxWidget.setEdit(true);

	// Set distance to window borders. Default is 4
	//boxWidget.setPads(25, 25);

	// Borders
	//boxWidget.setUseBorder(true);
	//boxWidget.setUseBorderBlinking(true); // forces above border too

	// Constraint box sizes
	//boxWidget.setRectConstraintMin(glm::vec2(50, 50)); // Min shape
	//boxWidget.setRectConstraintMax(glm::vec2(ofGetWidth() - 25, ofGetHeight() - 25)); // Max shape

	// Draggable borders
	//boxWidget.setLockW(true); // disable x/width
	//boxWidget.setLockH(true); // disable y/height
	//boxWidget.setBorderColor(c); // custom color

	// Force layout position
	//boxWidget.setMode(ofxSurfingBoxInteractive::BOTTOM_RIGHT);
	//boxWidget.setMode(ofxSurfingBoxInteractive::CENTER_LEFT);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Scene
	drawScene();

	// Box Interactive
	boxWidget.draw();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	// Draw an animated inner box 
	// attached to the box rectangle widget

	int d = 60;
	float s = ofMap(ofGetFrameNum() % d, 0, d, 0.5f, 1.f);

	// Here we get the ofRectangle from the Box object!
	ofRectangle r = boxWidget.getRectangle();

	// Animate scale
	r.scaleFromCenter(s);

	ofPushStyle();
	ofFill();
	ofSetColor(ofColor::blue);
	ofDrawRectangle(r);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ') boxWidget.setToggleEdit();
	if (key == OF_KEY_BACKSPACE) boxWidget.reset();

	if (key == OF_KEY_LEFT) boxWidget.setToggleMode(true);
	if (key == OF_KEY_RIGHT || key == OF_KEY_TAB) boxWidget.setToggleMode();

	if (key == 'A') boxWidget.setToggleLockAspectRatio();
	if (key == 'B') boxWidget.setToggleUseBorder();
	if (key == 'K') boxWidget.setToggleUseBorderBlinking();

	if (key == 'D') boxWidget.setToggleDebug();
	if (key == 'd') boxWidget.setToggleDebugDoubleClick();
}
