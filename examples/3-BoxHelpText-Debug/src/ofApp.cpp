#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowPosition(-1920, 30);

	//--

	// Custom path and/or name to avoid collide with other instances!
	textBoxWidget.setName("_ofAppName");
	textBoxWidget.setPath("ofAppPath");

	//// Set Light Theme. Is not stored on settings.
	//textBoxWidget.setTheme(false); // light

	//--

	//TODO: 
	// Customization is WIP yet,
	// now could break the layout...

	// Customize Fonts
	//if(0)
	{
		// Body Font 
		textBoxWidget.setFontName("AnonymousPro-Bold.ttf");
		textBoxWidget.setFontSize(14);

		// Tittle Font 
		textBoxWidget.setFontTitleName("UbuntuMono-Bold.ttf");
		textBoxWidget.setFontTitleSize(60);
	}

	// Customize Padding
	if (0)
	{
		textBoxWidget.setPadding(120);
		//textBoxWidget.setPadding(70);
		//textBoxWidget.setPadding(0);

		textBoxWidget.setRounded(50);
		//textBoxWidget.setRounded(0);
	}

	//--

	// Set a custom tittle
	// before calling setup()
	//if (0)
	{
		std::string helpTitle = "";
		helpTitle += "HELP \n";
		helpTitle += "BOX \n";
		textBoxWidget.setTitle(helpTitle);
	}

	//--

	textBoxWidget.setup();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofSetWindowTitle(textBoxWidget.getModeName());

	// Optional:
	// Maybe, with a callback, we could build the info only when we want to be updated.
	// Here is updated on every frame.
	buildHelpInfo();

	//// Optional:
	//// We could update our info depending of our app Mode during runtime.
	//// rebuild text info if required. 
	//// Or maybe when changed internally the theme or layout ...
	//if (textBoxWidget.isChanged()) buildHelpInfo();

	//--

	textBoxWidget.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == OF_KEY_TAB) textBoxWidget.setToggleLayoutMode();
	if (key == ' ') textBoxWidget.setToogleEdit();
	if (key == 'h') textBoxWidget.setToggleVisible();
	if (key == 't') textBoxWidget.setToggleTheme();
	if (key == 'm') bMinimized = !bMinimized;
}

//--------------------------------------------------------------
void ofApp::buildHelpInfo()
{
	//--

	std::string helpInfo = "";

	if (!bMinimized)
	{
		//TODO: WIP
		// When customizing fonts and sizes,
		// Sometimes we need to force spaces to fix layout troubles.
		helpInfo += "\n";
		helpInfo += "\n";

		// A tittle but into the body
		// with the same font as the body.
		//helpInfo += "HELP \n";
		//helpInfo += "ofxSurfingBoxHelpText \n";
		//helpInfo += "\n";


		bool b = textBoxWidget.getIsEditing();
		helpInfo += "DoubleClick to " + ofToString(b ? "LOCK" : "EDIT") + ". \n\n";
		if (b) {
			helpInfo += "LeftClick + RightClick to close.\n";
			helpInfo += "Drag the Box around the window!\n";
			helpInfo += "Layout will be auto saved.\n";
		}
		helpInfo += "\n\n";
		helpInfo += "KEY COMMANDS \n";
		helpInfo += "\n";
		helpInfo += "h      HELP \n";
		helpInfo += "\n";
		helpInfo += "t      TOGGLE THEME \n";
		helpInfo += "       " + textBoxWidget.getThemeName() + "\n";
		helpInfo += "\n";
	}

	helpInfo += "TAB    TOGGLE POSITION \n";
	helpInfo += "       " + textBoxWidget.getModeName() + "\n";
	helpInfo += "\n";
	helpInfo += "SPACE  TOGGLE EDIT " + ofToString(textBoxWidget.getIsEditing() ? "TRUE" : "FALSE") + "\n";
	helpInfo += "\n";
	helpInfo += "m      MINIMIZE \n";
	helpInfo += "\n";

	// Testing monospace
	if (bMinimized)
	{
		helpInfo += "\n";
		helpInfo += "1      WINDOW 1 ON \n";
		helpInfo += "2      WINDOW 2 ON \n";
		helpInfo += "3      WINDOW 3 ON \n";
		helpInfo += "4      WINDOW 4 ON \n";
		helpInfo += "5      WINDOW 5 ON \n";
		helpInfo += "6      WINDOW 6 ON \n";
	}

	textBoxWidget.setText(helpInfo);
}