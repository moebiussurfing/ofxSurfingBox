#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowPosition(-1920, 25);

	//--
	
	// Optional

	//// Customize to avoid collide 
	//// when using multiple instances!
	//boxWidget.setName("Demo"); // for the filename
	//boxWidget.setPath("MyAddon/"); // for the container folder

	//--

	boxWidget.setup();

	//--

	// Optional
	
	// Force edit
	boxWidget.setEdit(true);

	// Debug
	boxWidget.setDebug(true);

	// Borders
	//boxWidget.setUseBorder(true);
	//boxWidget.setUseBorderBlinking(true);

	// Force layout position
	//boxWidget.setMode(ofxSurfingBoxInteractive::FREE_LAYOUT);

	// Constraint sizes
	//boxWidget.setRectConstraintMin(glm::vec2(50, 50));
	//boxWidget.setRectConstraintMax(glm::vec2(ofGetWidth() - 25, ofGetHeight() - 25));

	// Draggable borders
	//boxWidget.setLockW(true); // disable width
	//boxWidget.setLockH(true); // disable height
	//boxWidget.setBorderColor(c);
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
	int d = 60;
	float s = ofMap(ofGetFrameNum() % d, 0, d, 0.5f, 1.f);

	//--

	// Draw a box attached to box rectangle

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
	if (key == 'A') boxWidget.setToggleLockAspectRatio();
	if (key == 'B') boxWidget.setToggleUseBorder();
	if (key == 'K') boxWidget.setToggleUseBorderBlinking();
	if (key == 'D') boxWidget.setToggleDebugDoubleClick();
	if (key == OF_KEY_TAB) boxWidget.setToggleMode();
	if (key == OF_KEY_BACKSPACE) boxWidget.reset();
}
