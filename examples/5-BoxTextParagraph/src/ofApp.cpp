#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowPosition(-1920, 23);

	//--

	//// Optional

	//// Customize to avoid collide 
	//// when using multiple instances!
	//boxWidget.setName("Demo"); // for the filename
	//boxWidget.setPath("MyAddon/"); // for the container folder

	//--

	boxWidget.setup();

	buildHelpInfo();
	//boxWidget.setPadX(50);
	//boxWidget.setPadY(100);
	//boxWidget.setPadX(0);
	//boxWidget.setPadY(0);
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (bModeSimple)
	{
		// Using simple text without formatting

		if (boxWidget.isChanged()) buildHelpInfo();
	}
	else 
	{
		// Using style tags

		string dynamicSizeTag =
			"<style font='F1' size='" +
			ofToString(18 + 1 * sinf(30 * ofGetElapsedTimef()), 2) +
			"' color='#888888'>";

		string blinkTag =
			"<style font='F2' size='18' color='#FFFFFF" +
			string(ofGetFrameNum() % 20 < 10 ? "11" : "EE") + "'>";

		string styledText =
			u8"<body>"
			"<br/><br/>"
			"<bold>ofxFontStash2</bold> allows you to draw text. It can draw individual "
			"Visitar l’àvia des del balcó."
			"Com farem les pràctiques des de casa?!"
			"Conèixer millor els veïns"
			"lines of text, or longer texts constrained to a column. "
			"It offers text alignment (left, center, right).<br/><br/>"
			"It also allows you to mix and match different text <italic>styles</italic> in the same "
			"paragraph. You can do so by <bold>creating styles that can be applied at an "
			"individual <bold>cha<bold>rac</bold>ter level</bold> with style nesting</bold>. You can also inline styles."
			"<br/><br/>"
			"Here, we demo the inline styles feature by dynamically "
			"setting a font " + dynamicSizeTag + "size.</style><br/>"
			"Here we show how to easily make text " + blinkTag + "blink.</style>"
			"<br/><br/>"
			"Here we test what happens when to styles are <bold>side</bold> <italic> by side. </italic>"
			"Here we test inline style <italic color='#66'>overrides.</italic>"
			"<br/><br/>"
			"Here we test a 1/2 lineHeight line break...<br/><br heightMult='0.5'/>"
			"And we are done testing. Let's go home."
			"</body>";

		boxWidget.setTitle("ofxSurfingBoxParagraph");
		//boxWidget.setTitle("ofxSurfingBoxParagraph\nFORMATTING");
		boxWidget.setText(styledText);
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	boxWidget.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	// switch mode
	if (key == OF_KEY_F1) {
		bModeSimple = !bModeSimple;

		boxWidget.setLockW(bModeSimple);
		//true bc we are not using a paragraph. we don't need formatting.
		// or false bc we want formatting dragging box width.
	}

	if (key == ' ') boxWidget.setToggleEdit();
	if (key == OF_KEY_BACKSPACE) boxWidget.reset();

	if (key == OF_KEY_LEFT) boxWidget.setToggleMode(true);
	if (key == OF_KEY_RIGHT || key == OF_KEY_TAB) boxWidget.setToggleMode();

	if (key == 'B') boxWidget.setToggleUseBorder();
	if (key == 'K') boxWidget.setToggleUseBorderBlinking();

	if (key == 'H') boxWidget.setToggleVisible();
	if (key == 'D') boxWidget.setToggleDebug();
	if (key == 'd') boxWidget.setToggleDebugDoubleClick();

	if (key == 'T') boxWidget.setToggleTheme();
}

//--------------------------------------------------------------
void ofApp::buildHelpInfo()
{
	// Using plain text without style tags

	helpInfo = "";

	helpInfo += "<header>";
	helpInfo += "ofxSurfingBox\nParagraph";
	helpInfo += "</header>";

	helpInfo += "<br/><br/>";
	//helpInfo += "\n\n";
	//helpInfo += "\n\n";

	helpInfo += "<H1>";
	bool b = boxWidget.isEditing();
	helpInfo += ofToString(!b ? "LOCKED! " : "EDITING!");
	helpInfo += "\nDoubleClick > " + ofToString(b ? "LOCK" : "EDIT") + ". \n\n";
	helpInfo += "</H1>";

	helpInfo += "<H3>";
	helpInfo += "Drag the Box around the window!\n";
	helpInfo += "LeftClick + RightClick to close.\n";
	helpInfo += "Layout will be auto saved.\n\n";
	helpInfo += "</H3>";
	
	helpInfo += "<br/>";

	helpInfo += "<header2>";
	helpInfo += "KEY COMMANDS \n";
	helpInfo += "\n";
	helpInfo += "</header2>";

	helpInfo += "<H2>";
	helpInfo += "SPACE    EDIT \n";
	helpInfo += "\n";
	helpInfo += "H        HELP \n";
	helpInfo += "\n";
	helpInfo += "T        TOGGLE THEME \n";
	helpInfo += "         > " + boxWidget.getThemeName() + "\n";
	helpInfo += "\n";
	helpInfo += "TAB      TOGGLE POSITION \n";
	helpInfo += "         > " + boxWidget.getModeName();
	helpInfo += "\n\n";
	helpInfo += "F1       TOGGLE TEXT MODE \n";
	helpInfo += "         > " + string(bModeSimple ? "SIMPLE" : "FORMATTING");
	helpInfo += "</H2>";

	//boxWidget.setTitle("ofxSurfingBoxParagraph");
	//boxWidget.setText(helpInfo);

	boxWidget.setTextStyled(helpInfo);
}