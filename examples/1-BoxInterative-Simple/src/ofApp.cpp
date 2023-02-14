#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	//ofSetWindowPosition(-1920, 23);

	//--

	// Optional
	// Customize to avoid collide
	// settings folders when using 
	// multiple instances!
	boxWidget.setName("Demo"); // for the filename
	boxWidget.setPath("MyAddon/"); // for the container folder

	//--

	boxWidget.setup();

	//--

	// Optional
	setupOptional();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Scene
	// We can link visibility with the widget
	if (boxWidget.isVisible()) drawSceneBoxed();

	// Box Interactive
	boxWidget.draw();

	boxWidget.drawHelpInfo();
}

//--------------------------------------------------------------
void ofApp::drawSceneBoxed()
{
	// Draw an animated inner box 
	// attached to the box rectangle widget

	// Here we get the ofRectangle from the Box object!
	ofRectangle r = boxWidget.getRectangle();

	// Animate scale
	float s = ofMap(glm::cos(8 * ofGetElapsedTimef()), -1, 1, 0.5, 1);
	r.scaleFromCenter(s);

	ofPushStyle();
	ofFill();
	ofSetColor(boxWidget.getModeLayout() == 0 ? ofColor::yellow : ofColor::blue);
	ofDrawRectRounded(r, 10);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ') boxWidget.setToggleEdit();
	if (key == OF_KEY_BACKSPACE) boxWidget.reset();

	if (key == OF_KEY_RIGHT || key == OF_KEY_TAB) boxWidget.setToggleMode();//next
	if (key == OF_KEY_LEFT) boxWidget.setToggleMode(true);//prev

	if (key == 'A') boxWidget.setToggleLockAspectRatio();
	if (key == 'B') boxWidget.setToggleUseBorder();
	if (key == 'K') boxWidget.setToggleUseBorderBlinking();

	if (key == 'G') boxWidget.setToggleVisible();
	if (key == 'D') boxWidget.setToggleDebug();
	if (key == 'd') boxWidget.setToggleDebugDoubleClick();
}

//--------------------------------------------------------------
void ofApp::setupOptional()
{
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