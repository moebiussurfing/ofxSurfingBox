
#pragma once

#include "ofMain.h"

#include "ofxSurfingHelpers.h"
#include "ofxInteractiveRect.h"
#include "DoubleClicker.h"

// Originally taken from ofxSurfingHelpers/gui/widgets/TextBoxWidget.h
// This class is more updated!

/*

Default fonts to be placed into dat/assets/fonts

JetBrainsMonoNL-ExtraBold.ttf
JetBrainsMono-ExtraBold.ttf

*/

/*

	TODO:

	+ fix broken layout when using a title with one line only!
		Now we must use two lines.
	+ use ctrl modifier bc three clicks interferes with double..

*/


/*

	This class draws a draggable text box with colored background.
	Auto stores and recall the box position between the app sessions.
	DoubleClick the box to allow move the position.
	Can be locked to window corners predefined places.
	Allows a title with a bigger font.
	title and body fonts can be customized.

*/

//--

// Extra Customization
/*

//TODO:
//boxHelpInfo.setFixedHeight(1);

 Call before call setup!
boxHelpInfo.setPath(path_GLOBAL + "HelpBox/");

 Customize Mode
//boxHelpInfo.setMode(ofxSurfingBoxHelpText::FREE_LAYOUT);
boxHelpInfo.setMode(ofxSurfingBoxHelpText::BOTTOM_CENTER);

// Custom Theme
boxHelpInfo.setTheme(true); // dark
//boxHelpInfo.setTheme(false); // light

*/

//--


//#define LOCK_EDIT_ON_NON_FREE_LAYOUT_MODE

class ofxSurfingBoxHelpText /* : public ofBaseApp*/
{

public:

	//--------------------------------------------------------------
	ofxSurfingBoxHelpText()
	{
		//TODO:
		//doubleClicker.enableAllEvents();

		//--

		// Default Fonts

		//--

		// Body Font

		size_TTF = 11;
		name_TTF = FONT_FILE_BIG;
		//name_TTF = "JetBrainsMonoNL-ExtraBold.ttf";
		//name_TTF = "JetBrainsMono-Bold.ttf";

		//--

		// Title Font

		size_TTF2 = 22;
		name_TTF2 = FONT_FILE_SMALL;
		//name_TTF2 = "JetBrainsMono-ExtraBold.ttf";

		//--

		/*
		//size_TTF = 10;
		//name_TTF = "telegrama_render.otf";

		//size_TTF = 12;
		//name_TTF = "Inconsolata-Bold.ttf";

		//size_TTF = 11;
		//name_TTF = "JetBrainsMonoNL-SemiBold.ttf";

		//size_TTF = 12;
		//name_TTF = "UbuntuMono-Bold.ttf";

		//name_TTF = "AnonymousPro-Bold.ttf";

		//name_TTF = "Inconsolata-Medium.ttf";
		//name_TTF = "Inconsolata-Regular.ttf";
		//name_TTF = "Inconsolata-Light.ttf";
		//name_TTF = "Inconsolata-ExtraLight.ttf";
		//name_TTF = "Inconsolata-Black.ttf";
		//name_TTF = "Inconsolata_Expanded-Black.ttf";

		//size_TTF = 14;
		//name_TTF = "Inconsolata_ExtraCondensed-Black.ttf";
		//
		//size_TTF = 10;
		//name_TTF = "overpass-mono-bold.otf";

		//size_TTF = 11;
		//name_TTF = "telegrama_render.otf";
		*/
	}

	//--------------------------------------------------------------
	~ofxSurfingBoxHelpText()
	{
		// All app settings
		ofxSurfingHelpers::CheckFolder(path_Global + "/");

		rect_HelpTextBox.saveSettings(nameBoxFile, path_Global + "/", false);

		ofxSurfingHelpers::saveGroup(params_AppSession, path_Global + "/Session" + nameBoxFile + ".xml");
		ofRemoveListener(params_AppSession.parameterChangedE(), this, &ofxSurfingBoxHelpText::Changed);
	}

	//--------------------------------------------------------------
	void setup()
	{
		ofLogNotice(__FUNCTION__);

		//string pathRoot = FONT_FILES_PATH;
		//string pathRoot = "assets/fonts/";
		// this is hardcoded. 
		// Put your font files there!
		// You can customize only the font file name.

		//path_TTF = FONT_FILES_PATH + ofToString(FONT_FILE_SMALL);
		//path_TTF = FONT_FILES_PATH + name_TTF;
		bool bLoaded = myFont.load(FONT_FILES_PATH + name_TTF, size_TTF, true, true);
		if (!bLoaded) {
			bLoaded = myFont.load(FONT_FILES_PATH + ofToString(FONT_FILE_BIG), size_TTF, true, true);
		}
		if (!bLoaded) {
			bLoaded = myFont.load(FONT_FILES_PATH + ofToString(FONT_FILE_SMALL), size_TTF, true, true);
		}
		if (!bLoaded) {
			bLoaded = myFont.load(FONT_FILES_PATH + ofToString(FONT_FILE_ALT1), size_TTF, true, true);
		}
		if (!bLoaded) {
			bLoaded = myFont.load(FONT_FILES_PATH + ofToString(FONT_FILE_ALT2), size_TTF, true, true);
		}
		if (!bLoaded) bLoaded = myFont.load(OF_TTF_MONO, size_TTF, true, true);

		if (bTitleSetted)
		{
			//path_TTF2 = FONT_FILES_PATH + name_TTF2;
			bool bLoaded2 = myFont2.load(FONT_FILES_PATH + name_TTF2, size_TTF2, true, true);
			if (!bLoaded) {
				bLoaded = myFont2.load(FONT_FILES_PATH + ofToString(FONT_FILE_BIG), size_TTF2, true, true);
			}
			if (!bLoaded) {
				bLoaded = myFont2.load(FONT_FILES_PATH + ofToString(FONT_FILE_SMALL), size_TTF2, true, true);
			}
			if (!bLoaded) {
				bLoaded = myFont2.load(FONT_FILES_PATH + ofToString(FONT_FILE_ALT1), size_TTF2, true, true);
			}
			if (!bLoaded) {
				bLoaded = myFont2.load(FONT_FILES_PATH + ofToString(FONT_FILE_ALT2), size_TTF2, true, true);
			}
			if (!bLoaded) bLoaded = myFont2.load(OF_TTF_MONO, size_TTF2, true, true);

			titleNumLines = getTitleHeightLines() + 1;
			ofRectangle _r(myFont2.getStringBoundingBox(textTitle, 0, 0));
			titleHeight = _r.getHeight() + 5;
		}

		//--

		//TODO: auto set padding...
		//round = 5;
		//padding = size_TTF * 20;

		//--

		_bUseShadow = true;

		setTheme(bThemeDarkOrLight);

		doubleClicker.set(0, 0, ofGetWidth(), ofGetHeight()); // default full screen
		doubleClicker.setDebug(false);
		doubleClicker.bGui.makeReferenceTo(bGui);

		//// Default position
		//reset();

		//----

		ofxSurfingHelpers::CheckFolder(path_Global + "/");

		// Load settings
		rect_HelpTextBox.loadSettings(nameBoxFile, path_Global + "/", false);

		// We don't need to resize, draggable borders and decoration.
		//rect_HelpTextBox.setLockResize(!bNoText);
		rect_HelpTextBox.setLockResize(true);
		rect_HelpTextBox.setTransparent();//no hide drawing
		rect_HelpTextBox.setEnableMouseWheel(false);//bc it's auto resized by amount of added text size!

		//--

		// Startup

		// For file the settings

		params_AppSession.setName("AppSession");
		params_AppSession.add(bThemeDarkOrLight);
		params_AppSession.add(index_ModeLayout);

		ofAddListener(params_AppSession.parameterChangedE(), this, &ofxSurfingBoxHelpText::Changed);

		ofxSurfingHelpers::loadGroup(params_AppSession, path_Global + "/Session" + nameBoxFile + ".xml");
	}

	//--------------------------------------------------------------
	void draw(string text)
	{
		setText(text);

		draw();
	}

	//--------------------------------------------------------------
	void draw()
	{
		if (!bGui) return;

		ofPushStyle();

		//----

		float _w = ofGetWidth();
		float _h = ofGetHeight();

		int _xx = 0;
		int _yy = 0;

		float _ww = 0;
		float _hh = 0;

		//--

		updateDoubleClicker();

		//--

		if (bNoText)
		{
			_ww = rect_HelpTextBox.getWidth();
			_hh = rect_HelpTextBox.getHeight();
		}
		else
		{
			_ww = ofxSurfingHelpers::getWidthBBtextBoxed(myFont, _ss, _padx);
			_hh = ofxSurfingHelpers::getHeightBBtextBoxed(myFont, _ss, _pady);

			rect_HelpTextBox.setHeight(_hh);
			rect_HelpTextBox.setWidth(_ww);
		}

		//--

		// Fit Marks

		//TODO:L must be fixed on ofxSurfingHelpers::drawTextBoxed
		//...manual correction could break layout... 
		// also when not using tittle breaks layout too...
		float offset = 0;
		//float offset = size_TTF + 0;

		ytop = padding / 2 - _pady - offset;
		ybottom = _h - _hh - _pady - _pady - padding / 2;

		xcenter = _w / 2 - _ww / 2 + _padx / 2;
		ycenter = _h / 2 - _hh / 2;

		xleft = _padx + padding / 2;
		xright = _w - _ww - padding / 2;

		// Force fit box inside the window
		doForceFitOnWindow();

		//--

		// Free

		if (index_ModeLayout.get() == FREE_LAYOUT)
		{
			_xx = rect_HelpTextBox.getX();
			_yy = rect_HelpTextBox.getY();
		}

		//-

		// Center 

		else if (index_ModeLayout.get() == CENTER) {

			_xx = xcenter;
			_yy = ycenter;
		}

		//--

		// Top 

		else if (index_ModeLayout.get() == TOP_LEFT) {
			_xx = xleft;
			_yy = ytop;
		}
		else if (index_ModeLayout.get() == TOP_CENTER) {
			_xx = xcenter;
			_yy = ytop;
		}
		else if (index_ModeLayout.get() == TOP_RIGHT) {
			_xx = xright;
			_yy = ytop;
		}

		//--

		// Bottom

		else if (index_ModeLayout.get() == BOTTOM_LEFT) {
			_xx = xleft;
			_yy = ybottom;
		}
		else if (index_ModeLayout.get() == BOTTOM_CENTER) {
			_xx = xcenter;
			_yy = ybottom;
		}
		else if (index_ModeLayout.get() == BOTTOM_RIGHT) {
			_xx = xright;
			_yy = ybottom;
		}

		//----

		ofColor colorBg;

		if (index_ModeLayout.get() == FREE_LAYOUT)
		{
			if (rect_HelpTextBox.isEditing())
			{
				float a = ofxSurfingHelpers::getFadeBlink(0.6f, 1.f);
				ofColor c = ofColor(_colorBg, _colorBg.a * a);

				rect_HelpTextBox.draw();

				colorBg = c;
			}
			else
			{
				colorBg = _colorBg;
			}
		}
		else
		{
			colorBg = _colorBg;
		}

		//--

		if (!bNoText)
		{
			//bFixedHeight = false;
			int h = (bFixedHeight ? hLocked : -1); // Unlocked to resize related to text size

			//--

			// Draw

			// Body Text

			ofxSurfingHelpers::drawTextBoxed(myFont, _ss,
				_xx, _yy,
				_colorText, colorBg, _bUseShadow, _colorShadow,
				padding, round, h, true);

			//--

			// Tittle Text

			if (bTitleSetted)
			{
				// Get shape

				int _xx2 = _xx;
				int _yy2 = _yy + titleHeight - size_TTF2;

				//--

				// Text shadow

				if (_bUseShadow)
				{
					ofSetColor(_colorShadow);
					myFont2.drawString(textTitle, _xx2 + 1, _yy2 + 1);
				}

				// Text

				ofSetColor(_colorText);
				myFont2.drawString(textTitle, _xx2, _yy2);
			}
		}

		//--

		doubleClicker.set(_xx, _yy, _ww, _hh);

		//--

		ofPopStyle();

		// Force fit box inside the window
		//doForceFitOnWindow();

		//--

		doubleClicker.draw();
	}

	//--

private:

	//--------------------------------------------------------------
	void Changed(ofAbstractParameter& e)
	{
		std::string name = e.getName();
		ofLogNotice(__FUNCTION__) << name << " : " << e;

		//--

		if (name == index_ModeLayout.getName())
		{
			bIsChanged = true;
		}

		if (name == bThemeDarkOrLight.getName())
		{
			setTheme(bThemeDarkOrLight);

			bIsChanged = true;
		}

		if (name == bGui.getName())
		{
			setLocked(!bGui);

			bIsChanged = true;
		}
	}

	bool bEnableMouseWheel = false;

public:

	void setEnableMouseWheel(bool b) { bEnableMouseWheel = b; }

	ofParameter<bool> bGui{ "Help", true }; //exposed toggle to be used or linked in other parent scope guis!

	// Example: 
// Can be linked before calling setup()
//boxHelpInfo.bGui.makeReferenceTo(ui.bHelp);

//-

public:

	enum BOX_LAYOUT
	{
		FREE_LAYOUT = 0,
		CENTER,
		TOP_LEFT,
		TOP_CENTER,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT,

		NUM_LAYOUTS
	};

private:

	DoubleClicker doubleClicker;//handles multiple mouse clicks and combinations

	ofParameter<int> index_ModeLayout{ "Layout Mode", 0, 0, NUM_LAYOUTS - 1 };
	string name_ModeLayout = "";

	bool bLocked = false;
	bool bIsChanged = false;

	// Fit Marks
	int xcenter;
	int ycenter;
	int xleft;
	int xright;
	int ytop;
	int ybottom;

	//--------------------------------------------------------------
	void doForceFitOnWindow()
	{
		if (rect_HelpTextBox.getY() > ybottom) // bottom
		{
			rect_HelpTextBox.setY(ybottom);
		}
		else if (rect_HelpTextBox.getX() < xleft) // left
		{
			rect_HelpTextBox.setX(xleft);
		}
		else if (rect_HelpTextBox.getX() > xright) // right
		{
			rect_HelpTextBox.setX(xright);
		}
		else if (rect_HelpTextBox.getY() < ytop) // top
		{
			rect_HelpTextBox.setY(ytop);
		}
	};

	ofParameterGroup params_AppSession{ "ofxSurfingBoxHelpText" };

public:

	// A simple callback to trig when theme or layout changed
	//--------------------------------------------------------------
	bool isChanged() {
		if (!bIsChanged) return false;
		else {
			bIsChanged = false;
			return true;
		}
	}

	//--------------------------------------------------------------
	string getEditingString() {
		if (rect_HelpTextBox.isEditing()) return "Editing"; else return "Not Editing";
	}

	//--------------------------------------------------------------
	bool getIsEditing() {
		return rect_HelpTextBox.isEditing();
	}

	//--------------------------------------------------------------
	string getThemeName() {
		if (bThemeDarkOrLight) return "Dark"; else return "Light";
	}

	//--------------------------------------------------------------
	string getTextMode() {
		if (bNoText) return "No Text"; else return "Text";
	}

	//--------------------------------------------------------------
	string getModeName() {
		name_ModeLayout = "UNKNOWN";

		//FREE_LAYOUT = 0,
		//CENTER,
		//TOP_LEFT,
		//TOP_CENTER,
		//TOP_RIGHT,
		//BOTTOM_LEFT,
		//BOTTOM_CENTER,
		//BOTTOM_RIGHT,

		switch (index_ModeLayout)
		{
		case 0: name_ModeLayout = "FREE_LAYOUT"; break;
		case 1: name_ModeLayout = "CENTER"; break;
		case 2: name_ModeLayout = "TOP_LEFT"; break;
		case 3: name_ModeLayout = "TOP_CENTER"; break;
		case 4: name_ModeLayout = "TOP_RIGHT"; break;
		case 5: name_ModeLayout = "BOTTOM_LEFT"; break;
		case 6: name_ModeLayout = "BOTTOM_CENTER"; break;
		case 7: name_ModeLayout = "BOTTOM_RIGHT"; break;
		default: name_ModeLayout = "UNKNOWN LAYOUT"; break;
		}
		return name_ModeLayout;
	}

private:

	ofxInteractiveRect rect_HelpTextBox = { "Help_ofxPresetsManager" };
	std::string nameBoxFile = "HelpBox";
	std::string path_Global = "ofxSurfingBoxHelpText/"; // can be setted before setup

	//--

	// font to label clicker boxes
	ofTrueTypeFont myFont;
	//std::string path_TTF;
	std::string name_TTF;
	int size_TTF;
	std::string textInfo = "ofxSurfingBoxHelpText \n empty content"; // info text to display shortcuts or path settings

	ofTrueTypeFont myFont2;
	//std::string path_TTF2;
	std::string name_TTF2;
	int size_TTF2;
	std::string textTitle = "NO TITLE"; // info text to display shortcuts or path settings

	bool bTitleSetted = false;
	int titleHeight = 0;
	int titleNumLines = 0;
	// estimated amount lines of the title, but measured on plain text lines height.
	// workaround
	// that's to discount into the box to make space on top!

	int _padx = 10;
	int _pady = 10;

	//TODO: should be called when the text is setted only.
	string _ss = "";

	//--

	ofColor _colorText; // lines and text color
	ofColor _colorBg; // background color
	ofColor _colorShadow; // bg selected button
	bool _bUseShadow;

	bool bCenter = true;
	bool bLeftPosition = false;

	bool bStateEdit = false;

	float round = 5;
	int padding = 50;

	bool bNoText = false;

	ofParameter<bool> bThemeDarkOrLight{ "Theme", true };

public:

	//--------------------------------------------------------------
	void setTextMode(bool b) {
		bNoText = !b;
		rect_HelpTextBox.setLockResize(b);
	}
	//--------------------------------------------------------------
	void setNoTextMode(bool b) {
		bNoText = b;
		rect_HelpTextBox.setLockResize(!b);
	}
	//--------------------------------------------------------------
	void setToggleNoTextMode() {
		bNoText = !bNoText;
		rect_HelpTextBox.setLockResize(!bNoText);
	}
	//--------------------------------------------------------------
	void setShape(ofRectangle shape) {
		rect_HelpTextBox.setRect(shape.x, shape.y, shape.getWidth(), shape.getHeight());
	}

public:

	//--------------------------------------------------------------
	void setPath(string path) { // Call before setup. Will set path to save settings into.
		path_Global = path;
	}
	//legacy
	//--------------------------------------------------------------
	void setPathGlobal(string path) { // Call before setup. Will set path to save settings into.
		path_Global = path;
	}

	//--------------------------------------------------------------
	void setName(string _name) { // Call before setup. Will custo name.
		nameBoxFile = _name;
	}

	//--------------------------------------------------------------
	void setRounded(int r = 5) {
		round = r;
	}
	//--------------------------------------------------------------
	void setPadding(int pad = 50) {
		padding = pad;
	}
	//--------------------------------------------------------------
	void setPaddingBorders(int pad = 10) {
		_padx = _pady = pad;
	}

	//--------------------------------------------------------------
	void setFontSize(int size = 10) { // Call before setup. Default it's 10
		size_TTF = size;
	}
	//--------------------------------------------------------------
	void setFontName(string name) { // Set the name only. path is /assets/fonts/*name*
		name_TTF = name;
	}

	//--------------------------------------------------------------
	void setFontTitleSize(int size = 10) { // Call before setup. Default it's 10
		size_TTF2 = size;
	}
	//--------------------------------------------------------------
	void setFontTitleName(string name) { // Set the name only. path is /assets/fonts/*name*
		name_TTF2 = name;
	}

	////--------------------------------------------------------------
	//void reset() {
	//	int sz = 200;
	//	rect_HelpTextBox.setPosition(ofGetWidth() / 2 - (sz / 2), ofGetHeight() / 2 - (sz / 2));
	//	rect_HelpTextBox.setWidth(sz);
	//	rect_HelpTextBox.setHeight(sz);
	//}

	//--------------------------------------------------------------
	int getTitleHeightLines() {

		ofRectangle _r(myFont2.getStringBoundingBox(textTitle, 0, 0));
		int _h = _r.getHeight();

		return (_h / size_TTF);
	}

	//-

private:

	//--------------------------------------------------------------
	void updateDoubleClicker()
	{
		//--

		// 1. Double click swap edit mode

		//-

		// Allow edit only on free layout mode:
#ifdef LOCK_EDIT_ON_NON_FREE_LAYOUT_MODE
		if (index_ModeLayout.get() == FREE_LAYOUT)
#endif
			//-

		{
			if (doubleClicker.isMouseDoubleClick())
			{
				bStateEdit = !bStateEdit;

				setEdit(bStateEdit);

				// workflow
				if (bStateEdit)
				{
					if (index_ModeLayout.get() != FREE_LAYOUT) index_ModeLayout = FREE_LAYOUT;
				}
				//modeLayout = FREE_LAYOUT;
			}
		}

		//--

		/*
		// 2. Triple clicks swap modeLayout mode

		if (doubleClicker.isMouseTripleClick())
		{
			int i = index_ModeLayout.get();
			i++;
			if (i >= NUM_LAYOUTS) { index_ModeLayout = FREE_LAYOUT; }//0
			else { index_ModeLayout = BOX_LAYOUT(i); }
		}
		*/

		//--

		// 4. Left pressed + right click : close box!

		if (ofGetMousePressed(0) && doubleClicker.isMouseRightClick()) {
			//if (doubleClicker.isMouseRightPressedThenPressedLeft()) {
			ofLogWarning("TextBoxWidget") << (__FUNCTION__);
			bGui = false;
		}

		//--

		// 3. Right click swap modeLayout mode

		else if (doubleClicker.isMouseRightClick())
		{
			if (getIsEditing())
			{
				ofLogNotice(__FUNCTION__) << "isMouseRightClick";

				int i = index_ModeLayout.get();
				i++;
				if (i >= NUM_LAYOUTS) { index_ModeLayout = FREE_LAYOUT; }//cycle to 0
				else { index_ModeLayout = BOX_LAYOUT(i); }
			}
		}

		//--

		//--
	}

public:

	//--------------------------------------------------------------
	void setDebug(bool b)//must call after setup!
	{
		doubleClicker.setDebug(b);
	}

	//--------------------------------------------------------------
	void setEdit(bool bEdit)
	{
		if (bEdit)
		{
			rect_HelpTextBox.enableEdit();

			if (index_ModeLayout.get() != FREE_LAYOUT) index_ModeLayout = FREE_LAYOUT;
		}
		else
		{
			rect_HelpTextBox.disableEdit();

			// All app settings
			rect_HelpTextBox.saveSettings(nameBoxFile, path_Global + "/", false);
		}
	}

	//--------------------------------------------------------------
	void setToogleEdit()
	{
		bool b = rect_HelpTextBox.isEditing();
		setEdit(!b);
	}

	//--------------------------------------------------------------
	void setToggleTheme() {
		bThemeDarkOrLight = !bThemeDarkOrLight;
		setTheme(bThemeDarkOrLight);
	}

	//--------------------------------------------------------------
	void setTheme(bool bTheme) {
		bThemeDarkOrLight = bTheme;

		// Light theme (false = light)
		if (!bThemeDarkOrLight)
		{
			_colorText = ofColor(0, 255);
			_colorShadow = ofColor(255, 64);
			_colorBg = ofColor(225, 64);
		}
		// Dark theme (white lines & black bg) (true = dark)
		else
		{
			_colorText = ofColor(255, 200);
			//_colorText = ofColor(255, 150);
			_colorShadow = ofColor(16, 225);
			_colorBg = ofColor(0, 200);
		}
	}

	//--------------------------------------------------------------
	void setToggleLayoutMode() {
		int i = BOX_LAYOUT(index_ModeLayout.get());
		i++;
		i = i % NUM_LAYOUTS;

		index_ModeLayout = i;
	}

	//--------------------------------------------------------------
	void setMode(BOX_LAYOUT mode) {
		index_ModeLayout = mode;
	}

	//--------------------------------------------------------------
	void setLocked(bool b) {
		bLocked = b;

		//if (b) doubleClicker.disableAllEvents();
		//else doubleClicker.enableAllEvents();
	}

	//--

	//--------------------------------------------------------------
	void setVisible(bool b) {
		bGui = b;
	}
	//--------------------------------------------------------------
	void setToggleVisible() {
		bGui = !bGui;
	}

public:

	//--------------------------------------------------------------
	// Must be called after	calling setup if using tittle!
	void setText(string text) {//erase text and create new text
		textInfo = text;

		std::string ss = textInfo;
		_ss = "\n";
		if (bTitleSetted)
		{
			string _l = "   \n";
			for (size_t i = 0; i < titleNumLines; i++) _ss += _l;
			_ss = _ss + ss;
		}
		else _ss = ss;
	}

	//--------------------------------------------------------------
	void addText(string text) {//queue text at end
		textInfo += text;
	}

	// Must be called before calling setup!
	// //TODO: BUG: sometimes happens some memory leaking?
	//--------------------------------------------------------------
	void setTitle(string text) {//must call before setup to correct layout!
		bTitleSetted = true;

		textTitle = "\n";
		textTitle += text;

		titleNumLines = getTitleHeightLines() + 1;
		ofRectangle _r(myFont2.getStringBoundingBox(textTitle, 0, 0));
		titleHeight = _r.getHeight() + 5;

		// Only required to update title on the fly!
		std::string ss = textInfo;
		_ss = "\n";
		if (bTitleSetted)
		{
			string _l = "   \n";
			for (size_t i = 0; i < titleNumLines; i++) _ss += _l;
			_ss = _ss + ss;
		}
		else _ss = ss;
	}

	//--

//	//TODO:
//	// Should be added to the ofxSurfingHelpers::drawTextBoxed, adding a bool flag to force height..
//	// A workaround to lock the box height to the amount of lines using a 'I' char, 
//	// then it will not depends to the chars of the text. 
//	// An using case could be an only one line text and his box height will no being different depending on used chars..

private:

	bool bFixedHeight = false;
	float hLocked = -1;
	int numLines = -1;

public:

	//TODO: workaround
	//--------------------------------------------------------------
	void setFixedHeight(int amntLines = 1) { // an small tweak to fix box size to an amount of lines
		bFixedHeight = true;
		numLines = amntLines;

		std::string _ss = "";
		for (int i = 0; i < numLines; i++) { _ss += "I" + (i == 0 && i < numLines - 1) ? "" : "\n"; }
		//std::string _ss = "I";
		hLocked = ofxSurfingHelpers::getHeightBBtextBoxed(myFont, _ss);
	}

};

//--