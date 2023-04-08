#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofxSurfingHelpers::setMonitorsLayout(1);

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
void ofApp::update()
{
	// We have a simple callback available
	if (boxWidget.isChanged()) { ofLog() << "boxWidget changed: " << boxWidget.getRectangle(); }
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Scene
	// Draw an animated inner box 
	// attached to the box rectangle widget

	// Here we get the ofRectangle from the Box object!
	ofRectangle r = boxWidget.getRectangle();

	// We can link visibility with the widget
	if (boxWidget.isVisible()) drawSceneBoxed(r);

	// Box Interactive controller
	boxWidget.draw();

	// Debug info
	boxWidget.drawHelpInfo();
}

//--------------------------------------------------------------
void ofApp::drawSceneBoxed(ofRectangle r)
{

	// Animate scale
	float s = ofMap(glm::cos(10 * ofGetElapsedTimef()), -1, 1, 0.98, 1);
	float d = MAX(r.getWidth() - r.getWidth() * s, (r.getHeight() - r.getHeight() * s));
	float w = r.getWidth();
	float h = r.getHeight();
	if (boxWidget.getModeType() != BOX_TYPE::TYPE_BAR_HORIZONTAL)
		w = r.getWidth() - d;
	if (boxWidget.getModeType() != BOX_TYPE::TYPE_BAR_VERTICAL)
		h = r.getHeight() - d;
	r.setFromCenter(r.getCenter(), w, h);

	// Draw
	ofPushStyle();
	ofFill();
	ofColor col = boxWidget.getModeLayout() == BOX_LAYOUT::FREE_LAYOUT ? ofColor::yellow : ofColor::blue;
	if (boxWidget.getModeType() != BOX_TYPE::TYPE_RECTANGLE) col = ofColor::green;
	ofSetColor(col);
	ofDrawRectRounded(r, 5);

	// Text label to debug box mode
	ofBitmapFont f;
	string ss = boxWidget.getModeLayoutName();
	ss += "\n" + ofToString(boxWidget.getModeTypeName());
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

	if (key == OF_KEY_RIGHT || key == OF_KEY_TAB) boxWidget.setToggleModeLayout();//next
	if (key == OF_KEY_LEFT) boxWidget.setToggleModeLayout(true);//prev

	if (key == 'A') boxWidget.setToggleLockAspectRatio();
	if (key == 'B') boxWidget.setToggleUseBorder();
	if (key == 'K') boxWidget.setToggleUseBorderBlinking();

	if (key == 'G') boxWidget.setToggleVisible();
	if (key == 'D') boxWidget.setToggleDebug();
	if (key == 'd') boxWidget.setToggleDebugDoubleClick();

	if (key == 'T') {
		boxWidget.setToggleType();
		boxWidget.reset();
	}
	if (key == 'R') boxWidget.setType(BOX_TYPE::TYPE_RECTANGLE);
	if (key == 'H') boxWidget.setType(BOX_TYPE::TYPE_BAR_HORIZONTAL);
	if (key == 'V') boxWidget.setType(BOX_TYPE::TYPE_BAR_VERTICAL);
}

//--------------------------------------------------------------
void ofApp::setupOptional()
{
	// List of some optional settings

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
	//boxWidget.setMode(BOX_LAYOUT::BOTTOM_RIGHT);
	//boxWidget.setMode(BOX_LAYOUT::CENTER_LEFT);
}