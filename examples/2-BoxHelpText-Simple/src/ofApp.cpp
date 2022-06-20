#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// Set a custom tittle
	// before calling setup()
	std::string helpTitle = "";
	helpTitle += "HELP \n";
	helpTitle += "BOX \n";
	textBoxWidget.setTitle(helpTitle);

	//--

	textBoxWidget.setup();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Maybe, with a callback, we could build the info only when we want to be updated.
	// Here is updated on every frame.
	buildHelpInfo();

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
	
	helpInfo += "DoubleClick to " + ofToString(b ? "LOCK" : "EDIT") + ". \n\n";
	helpInfo += "LeftClick + RightClick to close.\n\n";
	helpInfo += "Drag the Box around the window! \n\n";
	helpInfo += "Layout will be auto saved. \n\n";
	helpInfo += "\n";
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