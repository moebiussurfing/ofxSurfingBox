#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// Box
	boxWidget.setPath("myCustomHelpPath/");
	boxWidget.setup();
	boxWidget.setMode(ofxSurfingBoxInteractive::FREE_LAYOUT);

	// Scene
	colors.push_back(ofColor::red);
	colors.push_back(ofColor::green);
	colors.push_back(ofColor::blue);
	colors.push_back(ofColor::yellow);

	//--

	// Help Info
	textBoxWidget.setup();
	buildHelpInfo();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Help Info 

	// We could update the text when something on the class object changes...
	// Also for debug purposes.
	//if (textBoxWidget.isChanged()) buildHelpInfo();

	textBoxWidget.draw();

	//--

	// Box

	boxWidget.draw();

	//--
	 
	// Scene
	// To use the Box object.
	// We will draw a smaller/bigger rounded rectangle.
	// Animated and with changing color.
	{
		// Here we get the ofRectangle from the Box object!
		
		ofRectangle r = boxWidget.getRectangle();

		//-
		
		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(4);

		// rescale
		r.scaleFromCenter(ofxSurfingHelpers::getFadeBlink(0.5f, 0.7f, 0.12f));//inner
		//r.scaleFromCenter(ofxSurfingHelpers::getFadeBlink(1.1f, 1.2f, 0.12f));//outer
		
		bool b = (ofGetFrameNum() % 10 == 0);
		static ofColor c;
		if (b) // changes the color after 10 frames. 
		{
			static int k = 0;
			k = ++k % colors.size();
			c = colors[k];
		}
		ofSetColor(c, 128);
		
		ofDrawRectRounded(r, 20);
		
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == ' ') boxWidget.setToggleEdit();
	if (key == 'A') boxWidget.setToggleLockAspectRatio();
	if (key == OF_KEY_TAB) boxWidget.setToggleMode();
	if (key == OF_KEY_BACKSPACE) boxWidget.reset();
}

//--------------------------------------------------------------
void ofApp::buildHelpInfo()
{
	std::string helpInfo = "";
	
	helpInfo += "HELP \n";
	helpInfo += "\n";
	helpInfo += "KEY COMMANDS \n";
	helpInfo += "\n\n";
	helpInfo += "BACKSPACE      RESET \n";
	helpInfo += "DOUBLE CLICK   EDIT \n";
	helpInfo += "SPACE          EDIT \n";
	helpInfo += "A              TOGGLE LOCK ASPECT RATIO  \n";
	helpInfo += "TAB            MODE LAYOUT  \n";

	// We could update the text when something on the class object changes...
	// helpInfo += "DEBUG \n";
	// helpInfo += boxWidget.getMode();
	// helpInfo += "\n";
	// helpInfo += boxWidget.getEditing();
	// helpInfo += "\n";
	// helpInfo += "LOCKED ASPECT RATIO " + (string)(boxWidget.isLockedAspectRatio() ? "TRUE" : "FALSE");
	// helpInfo += "\n";

	textBoxWidget.setText(helpInfo);
}
