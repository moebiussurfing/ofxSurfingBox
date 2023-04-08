#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofxSurfingHelpers::setMonitorsLayout(1);

	boxWidget.setup();
}

//--------------------------------------------------------------
void ofApp::update()
{
	// We have a simple callback available
	if (boxWidget.isChanged()) { ofLog() << "boxWidget changed: "<< boxWidget.getRectangle(); }
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Scene
	// We can link visibility with the widget
	if (boxWidget.isVisible()) drawSceneBoxed();

	// Box Interactive controller 
	boxWidget.draw();
}

//--------------------------------------------------------------
void ofApp::drawSceneBoxed()
{
	if (!bDrawScene) return;

	// Draw an animated inner box 
	// attached to the Box Interactive widget

	// Here we get the ofRectangle from the Box object!
	ofRectangle r = boxWidget.getRectangle();

	// Animate scale
	r.scaleFromCenter(ofMap(glm::cos(20 * ofGetElapsedTimef()), -1, 1, 0.9, 1));

	ofPushStyle();
	ofFill();
	ofSetColor(ofColor::blue, 200);
	ofDrawRectangle(r);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ') boxWidget.setToggleEdit();
	if (key == OF_KEY_BACKSPACE) boxWidget.reset();
	if (key == OF_KEY_TAB) boxWidget.setToggleMode();

	if (key == 'd') bDrawScene = !bDrawScene;
}