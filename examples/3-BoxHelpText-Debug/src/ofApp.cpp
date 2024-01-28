#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//ofSetWindowPosition(-1920, 30);

	//--

	// Custom path and/or name to avoid collide with other instances!
	textBoxWidget.setName("_ofAppName");
	textBoxWidget.setPath("ofAppPath");

	//--

	//TODO:
	// Customization is WIP yet,
	// now could break the layout...

	// Customize Fonts
	if (0) // uncomment to use the default fonts.
	{
		// Body Font
		textBoxWidget.setFontName("AnonymousPro-Bold.ttf");
		textBoxWidget.setFontSize(14);

		// Tittle Font
		textBoxWidget.setFontTitleName("UbuntuMono-Bold.ttf");
		textBoxWidget.setFontTitleSize(60);
	}

	// Customize Style
	if (0) // uncomment to use the default styles.
	{
		textBoxWidget.setPads(0);
		textBoxWidget.setPadBox(0);
		textBoxWidget.setRounded(0);

		//textBoxWidget.setPadding(120);
		//textBoxWidget.setRounded(50);
	}

	//--

	// Set a custom tittle
	// before calling setup()
	//if (0) // uncomment to disable title.
	{
		std::string helpTitle = "";
		helpTitle += "HELP \n";
		helpTitle += "BOX \n";
		textBoxWidget.setTitle(helpTitle);
	}

	//--

	textBoxWidget.setup();

	//--

	// Enable Debug to double clicks and other stuff
	//textBoxWidget.setDebug(true);

	//// A. We set the help info on once, on setup,
	//// because we don't want to be changed during runtime.
	//buildHelpInfo();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetWindowTitle(textBoxWidget.getModeName());

	// B. We build the help info on every frame:
	// Maybe, with a callback, we could build the info only when we want to be updated.
	// Here is updated on every frame.
	buildHelpInfo();

	//// We could update our info depending of our app Mode during runtime.
	//// C. Rebuild text info if required.
	//if (bSomethingChanged()) buildHelpInfo();
	//// D. Or maybe when changed internally the theme or layout ...
	//if (textBoxWidget.isChanged()) buildHelpInfo();

	//--

	textBoxWidget.draw();

	//--

	//// D Can be directly draw without requiring to set to the object like when
	////textBoxWidget.setText(helpInfo);
	//draw(helpInfo)
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == OF_KEY_TAB) textBoxWidget.setToggleLayoutMode();
	if (key == ' ') textBoxWidget.setToogleEdit();
	if (key == 'h' || key == 'H') textBoxWidget.setToggleVisible();
	if (key == 'G') textBoxWidget.setToggleVisible();
	if (key == 'T') textBoxWidget.setToggleTheme();
	if (key == 'M') bMinimized = !bMinimized;
}

//--------------------------------------------------------------
void ofApp::buildHelpInfo() {
	//--

	std::string helpInfo = "";

	if (!bMinimized) {
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

		helpInfo += "POSITION \n" + textBoxWidget.getModeName() + "\n\n";

		helpInfo += ofToString(b ? "EDITING" : "LOCKED") + "\n";
		helpInfo += "DoubleClick to " + ofToString(b ? "LOCK" : "EDIT") + ". \n\n";
		if (b) {
			helpInfo += "Drag the Box around the window!\n";
			helpInfo += "RightClick to browse predefined Layouts.\n";
			helpInfo += "LeftClick + RightClick to close box.\n";
			helpInfo += "All will be auto saved.\n";
		}
		helpInfo += "\n\n";
		helpInfo += "KEY COMMANDS \n";
		helpInfo += "\n";
		helpInfo += "h/H    HELP \n";
		helpInfo += "\n";
		helpInfo += "T      TOGGLE THEME \n";
		helpInfo += "       " + textBoxWidget.getThemeName() + "\n";
		helpInfo += "\n";
	}

	helpInfo += "TAB    TOGGLE POSITION \n";
	helpInfo += "       " + textBoxWidget.getModeName() + "\n";
	helpInfo += "\n";
	helpInfo += "SPACE  EDIT/LOCK \n";
	helpInfo += "\n";
	helpInfo += "M      MINIMIZE \n";
	helpInfo += "\n";

	// Testing monospace
	if (bMinimized) {
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