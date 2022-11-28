#include "ofApp.h"

#define USE_TITLE 0

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowPosition(-1900, 100);

	// without title
	if (!USE_TITLE)
	{
		textBoxWidget.setup();
	}
	// using title  
	else
	{
		textBoxWidget.setup(true);

		// Set a custom tittle
		//helpTitle = "HELP BOX";
		helpTitle = "HELP\nBOX";
		//helpTitle = "HELP\nmyBOX\nAdd-on";

		textBoxWidget.setTitle(helpTitle);
	}

	//--
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// update
	//textBoxWidget.setTitle(helpTitle + " " + ofToString(ofGetFrameRate(), 0));

	// Maybe, with a callback, 
	// we could build the box info 
	// only when we want to be updated.
	// We use the own internal callback 
	// top detect that some internal stuff changed.
	if (textBoxWidget.isChanged())
	{
		buildHelpInfo();
	}

	//--

	textBoxWidget.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'h') textBoxWidget.setToggleVisible();
	if (key == 't') textBoxWidget.setToggleTheme();
	if (key == OF_KEY_TAB) textBoxWidget.setToggleLayoutMode();
}

//--------------------------------------------------------------
void ofApp::buildHelpInfo()
{
	// We could update our info depending of our App Mode during runtime.

	std::string helpInfo = "";

	bool b = textBoxWidget.getIsEditing();
	helpInfo += ofToString(!b ? "LOCKED! " : "EDITING!");
	helpInfo += "\nDoubleClick > " + ofToString(b ? "LOCK" : "EDIT") + ". \n\n";

	helpInfo += "LeftClick + RightClick to close.\n";
	helpInfo += "Drag the Box around the window!\n";
	helpInfo += "Layout will be auto saved.\n\n";

	helpInfo += "KEY COMMANDS \n";
	helpInfo += "\n";
	helpInfo += "h      HELP \n";
	helpInfo += "\n";
	helpInfo += "t      TOGGLE THEME \n";
	helpInfo += "       " + textBoxWidget.getThemeName() + "\n";
	helpInfo += "\n";
	helpInfo += "TAB    TOGGLE POSITION \n";
	helpInfo += "       " + textBoxWidget.getModeName() + "\n";

	textBoxWidget.setText(helpInfo);
}