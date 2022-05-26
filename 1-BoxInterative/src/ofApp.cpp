#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//--

	// API HELP
	{
		//// Font
		//textBoxWidget.setFontName("overpass-mono-bold.otf");
		//textBoxWidget.setFontSize(20);

		//TODO:
		////textBoxWidget.setFixedHeight(1); 

		// Customize path for storing settings 
		// (box position and size)
		// /Call before call setup!
		//textBoxWidget.setPath(path_GLOBAL + "HelpBox/");

		// -> Notice the calling place!
		//textBoxWidget.setup();

		// Customize Mode
		////textBoxWidget.setMode(ofxSurfingBoxHelpText::FREE_LAYOUT);
		//textBoxWidget.setMode(ofxSurfingBoxHelpText::BOTTOM_CENTER);

		//// Custom Theme
		//textBoxWidget.setTheme(true); // dark
		////textBoxWidget.setTheme(false); // light
	}

	//--

	textBoxWidget.setup();
	buildHelpInfo();
}

//--------------------------------------------------------------
void ofApp::buildHelpInfo()
{
	std::string helpInfo = "";
	helpInfo += "HELP \n";
	helpInfo += "ofxSurfingBoxHelpText \n";
	helpInfo += "\n";

	helpInfo += "Double click mouse to edit \n";
	helpInfo += "Drag the Box to desired place! \n";
	helpInfo += "Layout will be auto saved. \n";
	helpInfo += "\n";

	helpInfo += "KEY COMMANDS \n";
	helpInfo += "\n";
	helpInfo += "H          HELP \n";
	helpInfo += "\n";
	helpInfo += "T          TOGGLE THEME \n";
	helpInfo += "           " + textBoxWidget.getTheme() + "\n";
	helpInfo += "\n";

	helpInfo += "TAB        TOGGLE POSITION \n";
	helpInfo += "           " + textBoxWidget.getMode() + "\n";
	helpInfo += "\n";

	textBoxWidget.setText(helpInfo);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// OPTIONAL
	// rebuild text info if required. Or maybe when changed internally the theme or layouting...
	if (textBoxWidget.isChanged()) buildHelpInfo();

	textBoxWidget.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'h') textBoxWidget.setToggleVisible();
	if (key == 't') textBoxWidget.setToggleTheme();
	if (key == OF_KEY_TAB) textBoxWidget.setToggleMode();
}