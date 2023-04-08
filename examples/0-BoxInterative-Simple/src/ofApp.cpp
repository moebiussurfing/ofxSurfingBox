#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofxSurfingHelpers::setMonitorsLayout(1);

	boxWidget.setup();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Scene
	// We can link visibility with the widget
	if (boxWidget.isVisible() && bDrawScene) drawSceneBoxed();

	// Box Interactive
	boxWidget.draw();
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
	ofDrawRectRounded(r, 5);

	// Text label to debug box mode
	ofBitmapFont f;
	string ss = boxWidget.getModeLayoutName();
	auto bb = f.getBoundingBox(ss, 0, 0);
	glm::vec2 sz(bb.getWidth(), bb.getHeight());
	glm::vec2 c = glm::vec2(r.getCenter() - glm::vec2(sz.x / 2, sz.y / 2) + glm::vec2(0, 10));
	ofDrawBitmapStringHighlight(ss, c.x, c.y);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ') boxWidget.setToggleEdit();
	if (key == OF_KEY_BACKSPACE) boxWidget.reset();

	if (key == OF_KEY_RIGHT || key == OF_KEY_TAB) boxWidget.setToggleMode();//next
	if (key == OF_KEY_LEFT) boxWidget.setToggleMode(true);//prev
	if (key == 'd') bDrawScene = !bDrawScene;
}