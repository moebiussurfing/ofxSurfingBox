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

	scene.setColor1(255);
	scene.setColor2(0);

	//--

	// Help Info
	textBoxWidget.setup();
	buildHelpInfo();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Scene
	drawScene();

	//--

	// Box Help Text

	// Help Info 
	// We could update the text when something on the class object changes...
	// Also for debug purposes.
	if (textBoxWidget.isChanged()) buildHelpInfo();

	textBoxWidget.draw();

	//--

	// Box Interactive

	boxWidget.setBorderColor(c);
	boxWidget.draw();
	boxWidget.drawBorderBlinking();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	int d = 60;

	//float s = ofxSurfingHelpers::getFadeBlink(0.5f, 1.f, 0.04f);//inner
	//float s = ofxSurfingHelpers::getFadeBlink(1.1f, 1.2f, 0.04f);//outer

	float s = ofMap(ofGetFrameNum() % d, 0, d, 0.5f, 1.f);//inner

	bool b = (ofGetFrameNum() % d == 0);

	// Changes the color after an amount of d frames. 
	if (b)
	{
		static int k = 0;
		k = ++k % colors.size();
		c = colors[k];
	}

	// Draw a background scene
	if (0)
	{
		scene.drawScene();
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
void ofApp::keyPressed(int key) {

	if (key == ' ') boxWidget.setToggleEdit();
	if (key == 'A') boxWidget.setToggleLockAspectRatio();
	if (key == 'D') boxWidget.setToggleDebugDoubleClick();
	if (key == OF_KEY_TAB) boxWidget.setToggleMode();
	if (key == OF_KEY_BACKSPACE) boxWidget.reset();

	buildHelpInfo();
}

//--------------------------------------------------------------
void ofApp::buildHelpInfo()
{
	std::string helpInfo = "";

	helpInfo += "HELP \n";
	helpInfo += "\n";
	helpInfo += "KEY COMMANDS \n";
	helpInfo += "\n\n";
	helpInfo += "BACKSPACE      RESET SIZE \n";
	helpInfo += "\n";
	helpInfo += "TAB            MODE LAYOUT \n";
	helpInfo += "               " + ofToString(boxWidget.getModeLayoutString()) + " \n";
	helpInfo += "\n";

	if (boxWidget.getModeLayout() == ofxSurfingBoxInteractive::FREE_LAYOUT)
	{
		helpInfo += "FREE_LAYOUT \n";
		helpInfo += "\n";
		helpInfo += "SPACE \n";
		helpInfo += "DOUBLE-CLICK   " + ofToString(boxWidget.isEditing() ? "EDITING" : "LOCKED") + "\n";
		helpInfo += "\n";

		if (boxWidget.isEditing()) helpInfo += "MOUSE WHEEL    SCALE \n";
		else helpInfo += "\n";
		helpInfo += "\n";
		helpInfo += "D              DEBUG DOUBLE-CLICK " + ofToString(boxWidget.isDebugDoubleClick() ? "TRUE" : "FALSE");
		helpInfo += "\n\n";
		helpInfo += "A              LOCK ASPECT RATIO " + ofToString(boxWidget.isLockedAspectRatio() ? "TRUE" : "FALSE");
		helpInfo += "\n\n\n";
		helpInfo += "MOUSE WHEEL SCALE \n\n";
		helpInfo += "               FROM TOP LEFT \n";
		helpInfo += "+ CTRL         FROM CENTER \n";
		helpInfo += "+ SHIFT        WIDTH \n";
		helpInfo += "+ ALT          HEIGHT \n";
	}
	else helpInfo += "\n\n\n\n\n\n\n\n\n\n";

	helpInfo += "\n\n";
	helpInfo += "LAST UPDATE ON FRAME #" + ofToString(ofGetFrameNum());

	// We could update the text when something on the class object changes...
	// helpInfo += "DEBUG \n";
	// helpInfo += boxWidget.getMode();
	// helpInfo += "\n";
	// helpInfo += boxWidget.getEditingString();
	// helpInfo += "\n";
	// helpInfo += "LOCKED ASPECT RATIO " + (string)(boxWidget.isLockedAspectRatio() ? "TRUE" : "FALSE");
	// helpInfo += "\n";

	textBoxWidget.setText(helpInfo);
}
