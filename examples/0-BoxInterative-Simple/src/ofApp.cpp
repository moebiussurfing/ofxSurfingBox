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
	if (boxWidget.isChanged()) { ofLog() << "boxWidget changed: " << boxWidget.getRectangle(); }
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
	float s = ofMap(glm::cos(10 * ofGetElapsedTimef()), -1, 1, 0.98, 1);
	float d = MAX(r.getWidth() - r.getWidth() * s, (r.getHeight() - r.getHeight() * s));
	float w = r.getWidth();
	float h = r.getHeight();
	if (boxWidget.getModeType() != ofxSurfingBox::BOX_TYPE::TYPE_BAR_HORIZONTAL)
		w = r.getWidth() - d;
	if (boxWidget.getModeType() != ofxSurfingBox::BOX_TYPE::TYPE_BAR_VERTICAL)
		h = r.getHeight() - d;
	r.setFromCenter(r.getCenter(), w, h);

	// Draw
	ofPushStyle();
	ofFill();
	ofSetColor(ofColor::blue, 150);
	ofDrawRectangle(r);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'd') bDrawScene = !bDrawScene;

	if (key == ' ') boxWidget.setToggleEdit();
	if (key == OF_KEY_BACKSPACE) boxWidget.reset();
	if (key == OF_KEY_TAB) boxWidget.setToggleModeLayout();
}