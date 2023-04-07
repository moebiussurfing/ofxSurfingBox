
#pragma once

/*

	TODO:

	+ fix broken layout when not using title
	+ fix top less spacing when not using title

*/

//----

#include "ofMain.h"

#include "ofxSurfingHelpers.h"
#include "ofxInteractiveRect.h"
#include "DoubleClicker.h"

#define AUTO_CASE_CREATE(a) case a: return #a

// Originally taken from ofxSurfingHelpers/gui/widgets/TextBoxWidget.h
// This class is more updated!

//--

/*

	Default fonts to be placed into /bin/data/assets/fonts

	JetBrainsMonoNL-ExtraBold.ttf
	JetBrainsMono-ExtraBold.ttf

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

	//TODO: Currently layout broken if changed
	textBoxWidget.setPadBox(100);
	textBoxWidget.setPads(50);

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
		ofAddListener(ofEvents().exit, this, &ofxSurfingBoxHelpText::exit, OF_EVENT_ORDER_AFTER_APP);

		// Default Fonts

		//--

		// Body Font

		size_TTF = 11;
		name_TTF = FONT_FILE_BIG;
		//name_TTF = "JetBrainsMonoNL-ExtraBold.ttf";
		//name_TTF = "JetBrainsMono-Bold.ttf";

		//--

		// Title Font

		size_Style2 = 22;
		font_Style2 = FONT_FILE_SMALL;
		//font_Style2 = "JetBrainsMono-ExtraBold.ttf";
	}

	//--------------------------------------------------------------
	~ofxSurfingBoxHelpText()
	{
		ofRemoveListener(ofEvents().exit, this, &ofxSurfingBoxHelpText::exit);

		//exit();
	}

private:

	//--------------------------------------------------------------
	virtual void exit(ofEventArgs& args)
	{
		exit();
	}

	//--------------------------------------------------------------
	virtual void exit()
	{		
		// All app settings
		ofxSurfingHelpers::CheckFolder(path_Global + "/");

		rBox.saveSettings(nameBoxFile, path_Global + "/", false);

		ofxSurfingHelpers::saveGroup(params_AppSession, path_Global + "/" + nameBoxFile + "_Settings.xml");
		//ofxSurfingHelpers::saveGroup(params_AppSession, path_Global + "/Session" + nameBoxFile + ".xml");
		ofRemoveListener(params_AppSession.parameterChangedE(), this, &ofxSurfingBoxHelpText::Changed);
	}

public:

	//--------------------------------------------------------------
	//void setup(bool bUsingTitle = false)
	void setup()
	{
		ofLogNotice("ofxSurfingBoxHelpText") << (__FUNCTION__);

		//setUseTitle(bUsingTitle);

		// Fonts path is hardcoded "assets/fonts/";
		// Put your font files there!
		// You can customize only the font file name.
		// path = FONT_FILES_PATH + name;

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
		if (!bLoaded)
			bLoaded = myFont.load(OF_TTF_MONO, size_TTF, true, true);

		bool bLoaded2 = false;
		if (bUseTitle)
		{
			bLoaded2 = myFont2.load(FONT_FILES_PATH + font_Style2, size_Style2, true, true);
			if (!bLoaded2) {
				bLoaded = myFont2.load(FONT_FILES_PATH + ofToString(FONT_FILE_BIG), size_Style2, true, true);
			}
			if (!bLoaded2) {
				bLoaded2 = myFont2.load(FONT_FILES_PATH + ofToString(FONT_FILE_SMALL), size_Style2, true, true);
			}
			if (!bLoaded2) {
				bLoaded2 = myFont2.load(FONT_FILES_PATH + ofToString(FONT_FILE_ALT1), size_Style2, true, true);
			}
			if (!bLoaded2) {
				bLoaded2 = myFont2.load(FONT_FILES_PATH + ofToString(FONT_FILE_ALT2), size_Style2, true, true);
			}
			if (!bLoaded2)
				bLoaded2 = myFont2.load(OF_TTF_MONO, size_Style2, true, true);
		}

		//--

		//TODO: auto set tpad...
		//round = 5;
		//tpad = size_TTF * 20;

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
		rBox.loadSettings(nameBoxFile, path_Global + "/", false);

		// We don't need to resize, draggable borders and decoration.
		//rBox.setLockResize(!bNoText);
		rBox.setLockResize(true);
		rBox.setLockAspectRatio(false);
		rBox.setTransparent();//no hide drawing
		rBox.setEnableMouseWheel(false);//bc it's auto resized by amount of added text size!
		rBox.setPads(xpad, ypad);

		//--

		// Startup

		// For file the settings

		params_AppSession.setName("AppSession");
		params_AppSession.add(bThemeDarkOrLight);
		params_AppSession.add(index_ModeLayout);

		ofAddListener(params_AppSession.parameterChangedE(), this, &ofxSurfingBoxHelpText::Changed);

		//ofxSurfingHelpers::loadGroup(params_AppSession, path_Global + "/Session" + nameBoxFile + ".xml");
		ofxSurfingHelpers::loadGroup(params_AppSession, path_Global + "/" + nameBoxFile + "_Settings.xml");

		bIsChanged = true;

		bDoneSetup = true;
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
		if (ofGetFrameNum() == 0)
		{
			if (bUseTitle && !bReadyTitle)
			{
				if (bDoneSetup) doRefreshTitle();
			}
		}

		//--

		if (!bGui) return;

		ofPushStyle();
		ofSetCircleResolution(100);

		//----

		float _w = ofGetWidth();
		float _h = ofGetHeight();

		float _xx = 0;
		float _yy = 0;

		float _ww = 0;
		float _hh = 0;

		//--

		// make spacing for title
		string ss = "";
		if (bUseTitle)
		{
			//add empty lines
			string l = "   \n";
			for (size_t i = 0; i < amountLinesTitle; i++) ss += l;

			ss = ss + text_Body;
		}
		else ss = text_Body;

		//--

		if (!bNoText)
		{
			_ww = ofxSurfingHelpers::getWidthBBtextBoxed(myFont, ss, tpad);
			_hh = ofxSurfingHelpers::getHeightBBtextBoxed(myFont, ss, tpad);

			rBox.setHeight(_hh);
			rBox.setWidth(_ww);
		}
		else
		{
			_ww = rBox.getWidth();
			_hh = rBox.getHeight();
		}

		//--

		// Calculate Fit Marks

		//TODO:
		//must be fixed on ofxSurfingHelpers::drawTextBoxed
		//...manual correction could break layout... 
		// also when not using tittle breaks layout too...
		//float yoffset = tpad;

		{
			_ww = rBox.getWidth();
			_hh = rBox.getHeight();

			//round = 0;
			//round = 50;

			//tpad = 0;
			//xpad = 0;
			//ypad = 0;

			//tpad = 50;
			//xpad = 4;
			//ypad = 4;

			//tpad = 100;
			//xpad = 20;
			//ypad = 20;

			//rBox.setPads(xpad, ypad);

			xleft = xpad + tpad / 2;
			ytop = ypad + tpad / 2;

			xright = _w - _ww + tpad / 2 - xpad;
			ybottom = _h - _hh + tpad / 2 - ypad;

			xcenter = _w / 2.f - _ww / 2.f + tpad / 2;
			ycenter = _h / 2.f - _hh / 2.f + tpad / 2;
		}

		//--

		// Force fit box inside the window
		doForceFitOnWindow();

		//--

		// Free

		if (index_ModeLayout.get() == FREE_LAYOUT)
		{
			_xx = rBox.getX();
			_yy = rBox.getY();
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

		// Center 

		else if (index_ModeLayout.get() == CENTER_LEFT) {
			_xx = xleft;
			_yy = ycenter;
		}
		else if (index_ModeLayout.get() == CENTER_CENTER) {
			_xx = xcenter;
			_yy = ycenter;
		}
		else if (index_ModeLayout.get() == CENTER_RIGHT) {
			_xx = xright;
			_yy = ycenter;
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
			if (rBox.isEditing())
			{
				float a = ofxSurfingHelpers::getFadeBlink(0.6f, 1.f);
				ofColor c = ofColor(_colorBg, _colorBg.a * a);
				rBox.draw();
				colorBg = c;
			}
			else colorBg = _colorBg;
		}
		else colorBg = _colorBg;

		//--

		if (!bNoText)
		{
			//int h = (bFixedHeight ? hLocked : -1); // Unlocked to resize related to text size
			int h = -1;

			//--

			// Draw

			// 1. Body Text

			ofxSurfingHelpers::drawTextBoxed(myFont, ss,
				_xx, _yy,
				_colorText, colorBg, _bUseShadow, _colorShadow,
				tpad, round, h, true);//noPad

			//--

			// 2. Title Text

			if (bUseTitle)
			{
				int _xx2 = _xx;
				int _yy2 = _yy + tpad/2;

				//--

				// Text shadow
				if (_bUseShadow)
				{
					ofSetColor(_colorShadow);
					myFont2.drawString(text_Title, _xx2 + 1, _yy2 + 1);
				}

				// Text
				ofSetColor(_colorText);
				myFont2.drawString(text_Title, _xx2, _yy2);
			}
		}

		//--

		updateDoubleClicker();
		doubleClicker.set(_xx, _yy, _ww, _hh);
		doubleClicker.draw();

		//--

		ofPopStyle();
	}

	//--

private:

	//--------------------------------------------------------------
	void Changed(ofAbstractParameter& e)
	{
		string name = e.getName();
		ofLogNotice("ofxSurfingBoxHelpText") << (__FUNCTION__) << name << " : " << e;

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

public:

	ofParameter<bool> bGui{ "Help", true }; //exposed toggle to be used or linked in other parent scope guis!

	// Example: 
	// Can be linked before calling setup()
	//boxHelpInfo.bGui.makeReferenceTo(ui.bHelp);

	//-

public:

	enum BOX_LAYOUT
	{
		FREE_LAYOUT = 0,
		TOP_LEFT,
		TOP_CENTER,
		TOP_RIGHT,
		CENTER_LEFT,
		CENTER_CENTER,
		CENTER_RIGHT,
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
	float xcenter;
	float ycenter;
	float xleft;
	float xright;
	float ytop;
	float ybottom;

	//--------------------------------------------------------------
	void doForceFitOnWindow()
	{
		if (rBox.getY() > ybottom) // bottom
		{
			rBox.setY(ybottom);
		}
		else if (rBox.getX() < xleft) // left
		{
			rBox.setX(xleft);
		}
		else if (rBox.getX() > xright) // right
		{
			rBox.setX(xright);
		}
		else if (rBox.getY() < ytop) // top
		{
			rBox.setY(ytop);
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
		if (rBox.isEditing()) return "Editing"; else return "Not Editing";
	}

	//--------------------------------------------------------------
	bool getIsEditing() {
		return rBox.isEditing();
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
		BOX_LAYOUT modeLayout = BOX_LAYOUT(index_ModeLayout.get());
		switch (modeLayout) {
			AUTO_CASE_CREATE(FREE_LAYOUT);
			AUTO_CASE_CREATE(TOP_LEFT);
			AUTO_CASE_CREATE(TOP_CENTER);
			AUTO_CASE_CREATE(TOP_RIGHT);
			AUTO_CASE_CREATE(CENTER_LEFT);
			AUTO_CASE_CREATE(CENTER_CENTER);
			AUTO_CASE_CREATE(CENTER_RIGHT);
			AUTO_CASE_CREATE(BOTTOM_LEFT);
			AUTO_CASE_CREATE(BOTTOM_CENTER);
			AUTO_CASE_CREATE(BOTTOM_RIGHT);
		default: return "UNKNOWN LAYOUT";
		}
	}

	ofRectangle getRectangle() { return rBox.getRect(); };

	//--------------------------------------------------------------
	void setPosition(float x, float y) {
		rBox.setPosition(x, y);
	}
	
	//TODO: disable? must update?
	// disable
	//--------------------------------------------------------------
	void setShape(ofRectangle shape) {
		rBox.setRect(shape.x, shape.y, shape.getWidth(), shape.getHeight());
	}

private:

	ofxInteractiveRect rBox = { "Help_ofxPresetsManager" };

	string nameBoxFile = "HelpBox";
	string path_Global = "ofxSurfingBoxHelpText/"; // can be setted before setup

	//--

	ofTrueTypeFont myFont;
	string name_TTF;
	float size_TTF;

	ofTrueTypeFont myFont2;
	string font_Style2;
	float size_Style2;

	string text_Body = "";
	string text_Title = "NO TITLE"; // info text to display shortcuts or path settings

	bool bUseTitle = false;
	bool bReadyTitle = false;
	bool bDoneSetup = false;

	// workaround
	// Estimated amount lines of the title, but measured on plain text lines height.
	// that's to discount into the box to make space on top!
	int amountLinesTitle = 0;
	//int titleHeight = 0;

	float xpad = 4;
	float ypad = 4;

	// to inner text
	float tpad = 50;

	float round = 5;

	//--

	ofColor _colorText; // lines and text color
	ofColor _colorBg; // background color
	ofColor _colorShadow; // bg selected button
	bool _bUseShadow;

	bool bCenter = true;
	bool bLeftPosition = false;

	bool bStateEdit = false;

	bool bNoText = false;

	ofParameter<bool> bThemeDarkOrLight{ "Theme", true };

public:

	//--------------------------------------------------------------
	void setUseTitle(bool b) {
		bUseTitle = b;
	}

private: // disable
	//--------------------------------------------------------------
	void setTextMode(bool b) {
		bNoText = !b;
		rBox.setLockResize(b);
	}
	//--------------------------------------------------------------
	void setNoTextMode(bool b) {
		bNoText = b;
		rBox.setLockResize(!b);
	}
	//--------------------------------------------------------------
	void setToggleNoTextMode() {
		bNoText = !bNoText;
		rBox.setLockResize(!bNoText);
	}

//	//public:
//private: 
//	// disable
//	//--------------------------------------------------------------
//	void setShape(ofRectangle shape) {
//		rBox.setRect(shape.x, shape.y, shape.getWidth(), shape.getHeight());
//	}

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
	void setPadBox(int pad = 50) { // Inner padding between box borders an text paragraph
		tpad = pad;
	}
	//--------------------------------------------------------------
	void setPads(int pad = 4) { // Pad from box to window borders
		xpad = ypad = pad;
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
		size_Style2 = size;
	}
	//--------------------------------------------------------------
	void setFontTitleName(string name) { // Set the name only. path is /assets/fonts/*name*
		font_Style2 = name;
	}

	//--------------------------------------------------------------
	int getTitleAmountLinesUsingBodyFont(string str) {//get amount of title lines but on body font units. to add that spacing after!

		ofRectangle _r(myFont2.getStringBoundingBox(str, 0, 0));
		float _h = _r.getHeight();

		return (_h / (float)size_TTF);
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
			ofLogWarning("ofxSurfingBoxHelpText") << (__FUNCTION__);
			bGui = false;

			bIsChanged = true;
		}

		//--

		// 3. Right click swap modeLayout mode

		else if (doubleClicker.isMouseRightClick())
		{
			if (getIsEditing())
			{
				ofLogNotice("ofxSurfingBoxHelpText") << (__FUNCTION__) << "isMouseRightClick";

				int i = index_ModeLayout.get();
				i++;
				if (i >= NUM_LAYOUTS) { index_ModeLayout = FREE_LAYOUT; }//cycle to 0
				else { index_ModeLayout = BOX_LAYOUT(i); }
			}
		}

		//--
	}

public:

	//--------------------------------------------------------------
	void setDebug(bool b)//must call after setup!
	{
		doubleClicker.setDebug(b);

		bIsChanged = true;
	}

	//--------------------------------------------------------------
	void setEdit(bool bEdit)
	{
		if (bEdit)
		{
			rBox.enableEdit();

			if (index_ModeLayout.get() != FREE_LAYOUT) index_ModeLayout = FREE_LAYOUT;
		}
		else
		{
			rBox.disableEdit();

			// All app settings
			rBox.saveSettings(nameBoxFile, path_Global + "/", false);
		}

		bIsChanged = true;
	}

	//--------------------------------------------------------------
	void setToogleEdit()
	{
		bool b = rBox.isEditing();
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

		bIsChanged = true;
	}

	//--------------------------------------------------------------
	void setToggleLayoutMode() {
		int i = BOX_LAYOUT(index_ModeLayout.get());
		i++;
		i = i % NUM_LAYOUTS;

		index_ModeLayout = i;

		bIsChanged = true;
	}

	//--------------------------------------------------------------
	void setMode(BOX_LAYOUT mode) {
		index_ModeLayout = mode;

		bIsChanged = true;
	}

	//--------------------------------------------------------------
	void setLocked(bool b) {
		bLocked = b;

		//if (b) doubleClicker.disableAllEvents();
		//else doubleClicker.enableAllEvents();

		bIsChanged = true;
	}

	//--

	//--------------------------------------------------------------
	void setVisible(bool b) {
		bGui = b;

		bIsChanged = true;
	}
	//--------------------------------------------------------------
	void setToggleVisible() {
		bGui = !bGui;

		bIsChanged = true;
	}

public:

	//--------------------------------------------------------------
	// Must be called after	calling setup if using tittle!
	void setText(string text) {//erase text and create new text
		text_Body = text;

		//text_Body = "\n";
		//if (bUseTitle)
		//{
		//	string _l = "   \n";
		//	for (size_t i = 0; i < amountLinesTitle; i++) text_Body += _l;
		//	text_Body = text_Body + ss;
		//}
		//else text_Body = ss;
	}

	//--------------------------------------------------------------
	void addText(string text) {//queue text at end
		text_Body += text;
	}

	//TODO: BUG: sometimes happens some memory leaking?
	// Must be called before calling setup!
	//--------------------------------------------------------------
	void setTitle(string text) {//must call before setup to correct layout!
		bUseTitle = true;

		text_Title = text;

		if (bDoneSetup) doRefreshTitle();
	}

private:

	//--------------------------------------------------------------
	void doRefreshTitle()
	{
		amountLinesTitle = getTitleAmountLinesUsingBodyFont(text_Title);
		//amountLinesTitle += 1; // extra spacing
		//amountLinesTitle += 1; // extra spacing

		if (amountLinesTitle < 3) amountLinesTitle += 2; // extra spacing
		else if (amountLinesTitle < 6) amountLinesTitle += 1; // extra spacing
		// fix workaround to avoid too big space

		//TODO:
		//ofRectangle _r(myFont2.getStringBoundingBox(text_Title, 0, 0));
		//titleHeight = _r.getHeight();
		//titleHeight += 5;//pad

		bReadyTitle = true;
	}

	//--

	//TODO:
	// Should be added to the ofxSurfingHelpers::drawTextBoxed, adding a bool flag to force height..
	// A workaround to lock the box height to the amount of lines using a 'I' char, 
	// then it will not depends to the chars of the text. 
	// An using case could be an only one line text and his box height will no being different depending on used chars..

private:

	bool bFixedHeight = false;
	float hLocked = -1;
	int numLines = -1;

public:

	//TODO: workaround
	// An small tweak to fix box size to an amount of lines
	//--------------------------------------------------------------
	void setFixedHeight(int amntLines = 1) {
		bFixedHeight = true;
		numLines = amntLines;

		string _ss = "";
		for (int i = 0; i < numLines; i++) { _ss += "I" + (i == 0 && i < numLines - 1) ? "" : "\n"; }
		//string _ss = "I";
		hLocked = ofxSurfingHelpers::getHeightBBtextBoxed(myFont, _ss);
	}

};

//--