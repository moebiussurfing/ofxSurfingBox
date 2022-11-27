#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// Box
	boxWidget.setName("Demo");
	boxWidget.setPath("MyPath/");
	boxWidget.setup();
	boxWidget.setUseBorder(true);
	boxWidget.setMode(ofxSurfingBoxInteractive::FREE_LAYOUT);
	boxWidget.setRectConstraintMin(glm::vec2(50, 50));

	// Scene
	colors.push_back(ofColor::red);
	colors.push_back(ofColor::green);
	colors.push_back(ofColor::blue);
	colors.push_back(ofColor::yellow);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Scene
	drawScene();

	// Box Interactive
	boxWidget.setBorderColor(c);
	boxWidget.draw();
	boxWidget.drawBorderBlinking();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	int d = 60;

	float s = ofMap(ofGetFrameNum() % d, 0, d, 0.5f, 1.f);//inner
	//float s = ofMap(ofGetFrameNum() % d, 0, d, 1.1f, 1.2f);//outer

	bool b = (ofGetFrameNum() % d == 0);

	// Changes the color after an amount of d frames. 
	if (b)
	{
		static int k = 0;
		k = ++k % colors.size();
		c = colors[k];
	}

	//--

	// Draw a box attached to box rectangle

	// Here we get the ofRectangle from the Box object!
	ofRectangle r = boxWidget.getRectangle();

	// rescale
	r.scaleFromCenter(s);

	ofPushStyle();
	ofFill();
	//ofNoFill();
	//ofSetLineWidth(4);
	ofSetColor(c);
	ofDrawRectRounded(r, 0);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) 
{
	if (key == ' ') boxWidget.setToggleEdit();
	if (key == 'A') boxWidget.setToggleLockAspectRatio();
	if (key == 'D') boxWidget.setToggleDebugDoubleClick();
	if (key == OF_KEY_TAB) boxWidget.setToggleMode();
	if (key == OF_KEY_BACKSPACE) boxWidget.reset();
}
