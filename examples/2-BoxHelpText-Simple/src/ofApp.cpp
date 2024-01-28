#include "ofApp.h"

#define USE_TITLE 1 // Set to 1 or 0 to use title or not 

//--------------------------------------------------------------
void ofApp::setup()
{
	// Optional
	
	textBoxWidget.setName("Demo");
	textBoxWidget.setPath("myAddon");

	//textBoxWidget.setRounded(0);
	//textBoxWidget.setRounded(50);

	// Without title
	if (!USE_TITLE)
	{
		textBoxWidget.setup();
	}
	// Using title  
	else
	{
		// Pick one
		//helpTitle = "HELP BOX";//one line
		helpTitle = "HELP\nBOX";//two lines
		//helpTitle = "HELP\nmyBOX\nAdd-on";//three lines

		textBoxWidget.setTitle(helpTitle);

		textBoxWidget.setup();
	}
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

	if (key == ' ') textBoxWidget.setToogleEdit();
	if (key == 'H') textBoxWidget.setToggleVisible();
	if (key == 'T') textBoxWidget.setToggleTheme();
	if (key == OF_KEY_TAB) textBoxWidget.setToggleLayoutMode();
}

//--------------------------------------------------------------
void ofApp::buildHelpInfo()
{
	// We could update our info depending of our App Mode during runtime.

	std::string helpInfo = "";

	bool b = textBoxWidget.getIsEditing();
	helpInfo += ofToString(!b ? "LOCKED! " : "EDITING!");
	helpInfo += "\nDoubleClick > " + ofToString(b ? "LOCK" : "EDIT") + ".\n";
	helpInfo += "\n";
	helpInfo += "LeftClick + RightClick to close.\n";
	helpInfo += "Drag the Box around the window!\n";
	helpInfo += "Layout will be auto saved.\n";
	helpInfo += "\n";
	helpInfo += "KEY COMMANDS \n";
	helpInfo += "\n";
	helpInfo += "SPACE  EDIT \n";
	helpInfo += "\n";
	helpInfo += "H      HELP \n";
	helpInfo += "\n";
	helpInfo += "T      TOGGLE THEME \n";
	helpInfo += "       > " + textBoxWidget.getThemeName() + "\n";
	helpInfo += "\n";
	helpInfo += "TAB    TOGGLE POSITION \n";
	helpInfo += "       > " + textBoxWidget.getModeName();

	textBoxWidget.setText(helpInfo);
}