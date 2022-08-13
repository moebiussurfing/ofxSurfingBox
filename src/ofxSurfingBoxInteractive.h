
#pragma once

#include "ofMain.h"

#include "ofxSurfingHelpers.h"
#include "ofxInteractiveRect.h"
#include "DoubleClicker.h"

/*

	This class is a draggable ofRectangle based (not purely derived)
	It can be used as a viewport or like any rectangle on many scenarios.
	Can be placed into pre determined top, bottom, left... positions.
	Also can be positioned manually.
	Mouse wheel changes scale.
	Aspect ratio can be locked.
	Double click activates editing or locked modes.
	Can be also handled with double, triple mouse clicks.

*/


/*


	TODO:

	+ add click + space to easy edit/lock
	+ add all settings -> get from text box
	+ save mode too -> on text box is implemented!
	+ fit screen or mini/ big modes to use on a video player.

*/


//-


class ofxSurfingBoxInteractive /* : public ofBaseApp*/
{

public:

	//--------------------------------------------------------------
	ofxSurfingBoxInteractive()
	{
	}

	//--------------------------------------------------------------
	~ofxSurfingBoxInteractive()
	{
		// All app settings
		ofxSurfingHelpers::CheckFolder(path_Global + "/");
		rect_Box.saveSettings(path_RectHelpBox, path_Global + "/", false);

		bEdit.removeListener(this, &ofxSurfingBoxInteractive::Changed_Edit);
	}

	ofParameter<bool> bGui{ "Box Rectangle", true };//exposed toggle to be used or linked in other parent scope guis!

	//-

public:

	enum BOX_LAYOUT
	{
		FREE_LAYOUT = 0,
		BOTTOM_CENTER,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		TOP_CENTER,
		TOP_LEFT,
		TOP_RIGHT,
		CENTER,

		NUM_LAYOUTS
	};

private:

	// Fit Marks
	int xcenter;
	int ycenter;
	int xleft;
	int xright;
	int ytop;
	int ybottom;

	//--

	// DoubleClick engine

private:

	DoubleClicker doubleClicker;

	BOX_LAYOUT modeLayout = FREE_LAYOUT;
	string str_modeLayout = "";

	bool bLocked = false;
	bool bLockedAspectRatio = false;
	bool bIsChanged = false;

	bool bDebugDoubleClick = false;

	ofParameterGroup params_AppSession{ "AppSession" };

private:

	bool bEnableMouseWheel = true;

public:

	void setEnableMouseWheel(bool b) { bEnableMouseWheel = b; }

public:

	// Debug DoubleClick
	//--------------------------------------------------------------
	void setDebugDoubleClick(bool b) {
		bDebugDoubleClick = b;
		doubleClicker.setDebug(bDebugDoubleClick);
	}
	//--------------------------------------------------------------
	void setToggleDebugDoubleClick() {
		bDebugDoubleClick = !bDebugDoubleClick;
		doubleClicker.setDebug(bDebugDoubleClick);
	}
	//--------------------------------------------------------------
	bool isDebugDoubleClick() {
		return bDebugDoubleClick;
	}

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
	bool isEditing() {

		bool b = bIsEditing;

		return b;
	}

	//--------------------------------------------------------------
	BOX_LAYOUT getModeLayout() {
		return modeLayout;
	}

	//--------------------------------------------------------------
	string getModeLayoutString() {
		str_modeLayout = "UNKNOWN";
		switch (modeLayout)
		{
		case 0: str_modeLayout = "FREE_LAYOUT"; break;
		case 1: str_modeLayout = "BOTTOM_CENTER"; break;
		case 2: str_modeLayout = "BOTTOM_LEFT"; break;
		case 3: str_modeLayout = "BOTTOM_RIGHT"; break;
		case 4: str_modeLayout = "TOP_CENTER"; break;
		case 5: str_modeLayout = "TOP_LEFT"; break;
		case 6: str_modeLayout = "TOP_RIGHT"; break;
		case 7: str_modeLayout = "CENTER"; break;
		default: str_modeLayout = "UNKNOWN LAYOUT"; break;
		}
		return str_modeLayout;
	}

	ofxInteractiveRect rect_Box = { "_" };

	//--------------------------------------------------------------
	ofRectangle getRectangle() {
		return rect_Box;
	}
	float getX() { return rect_Box.x; }
	float getY() { return rect_Box.y; }
	float getWidth() { return rect_Box.getWidth(); }
	float getHeight() { return rect_Box.getHeight(); }

	void setUseBorder(bool b) { bUseBorder = b; }
private:
	bool bUseBorder = false;

	std::string path_RectHelpBox = "myBox";
	std::string path_Global = "ofxSurfingBoxInteractive/"; // can be setted before setup
	//std::string path_Name = "appSettings"; // subfolder for app session settings

	ofColor _colorButton;// bg selected button
	ofColor _colorBg;// background color
	ofColor _colorShadow;
	bool _bUseShadow;

	bool bCenter = true;
	bool bLeftPosition = false;

	bool bStateEdit = false;
	bool bState2 = false;

	float round = 5;
	//int marginBorders = 50;

	int _padx;
	int _pady;

	bool bIsEditing = false;

	//--

public:

	ofParameter<bool> bEdit;

	//--------------------------------------------------------------
	void setName(string name) {
		path_RectHelpBox = name;
	}

	//--

	//--------------------------------------------------------------
	void setRectangle(ofRectangle shape) {
		this->setShape(shape);
	}
	//--------------------------------------------------------------
	void setShape(ofRectangle shape) {
		rect_Box.setRect(shape.x, shape.y, shape.getWidth(), shape.getHeight());
	}
	//--------------------------------------------------------------
	void setPosition(float x, float y) {
		rect_Box.setX(x);
		rect_Box.setY(y);
	}
	//--------------------------------------------------------------
	void setWidth(float w) {
		rect_Box.setWidth(w);
	}
	//--------------------------------------------------------------
	void setHeight(float h) {
		rect_Box.setHeight(h);
	}

	//--

public:

	// Legacy
	//--------------------------------------------------------------
	void setPath(string path) {//call before setup. Will set path to save settings into.
		path_Global = path;
	}

	//--------------------------------------------------------------
	void setPathGlobal(string path) {//call before setup. Will set path to save settings into.
		path_Global = path;
	}

	//--------------------------------------------------------------
	void reset(bool bOnlySize = true, int width = 320) {

		int sz = width;
		rect_Box.setWidth(sz);
		rect_Box.setHeight(sz);
		if (!bOnlySize) rect_Box.setPosition(ofGetWidth() / 2 - (sz / 2), ofGetHeight() / 2 - (sz / 2));
	}

	//--------------------------------------------------------------
	void setup()
	{
		bEdit.set("EDIT BOX", false);
		bEdit.addListener(this, &ofxSurfingBoxInteractive::Changed_Edit);

		doubleClicker.set(0, 0, ofGetWidth(), ofGetHeight()); // default full screen

		doubleClicker.setDebug(bDebugDoubleClick);

		// Default position
		reset(false);

		// Fit Marks
		// padding to borders
		_padx = 0;
		_pady = _padx;

		//----

		ofxSurfingHelpers::CheckFolder(path_Global + "/");

		// Load settings
		rect_Box.loadSettings(path_RectHelpBox, path_Global + "/", false);

		rect_Box.setEnableMouseWheel(true);

		//--

		//// We dont need draggable borders and decoration.
		//rect_Box.setLockResize(true);
		////rect_Box.setLockResize(!bNoText);
		//rect_Box.setTransparent();
	}
	//--------------------------------------------------------------
	void Changed_Edit(bool& edit) {
		ofLogNotice(__FUNCTION__) << "Edit : " << edit;
		setEdit(edit);
	}

	//--------------------------------------------------------------
	void drawBorder(ofColor color = ofColor(0, 255))
	{
		// Border
		ofPushStyle();
		ofNoFill();
		ofSetColor(color);
		ofSetLineWidth(2.0);
		ofDrawRectangle(getRectangle());
		ofPopStyle();
	}

	//--------------------------------------------------------------
	void drawBorderBlinking(ofColor color = ofColor(255))
	{
		int a = ofMap(ofxSurfingHelpers::Bounce(), 0, 1, 24, 64);
		ofColor c = ofColor(color, a);
		drawBorder(c);
	}

	//--------------------------------------------------------------
	void draw()
	{
		ofPushStyle();

		//--

		// Simple callbacks

		static ofRectangle rect_Box_PRE = rect_Box.getRect();
		static BOX_LAYOUT modeLayout_PRE = NUM_LAYOUTS;
		static bool bLockedAspectRatio_PRE = false;

		// size changed
		if (rect_Box.getRect() != rect_Box_PRE) {
			rect_Box_PRE = rect_Box.getRect();
			bIsChanged = true;
		}

		// aspect changed
		if (bLockedAspectRatio != bLockedAspectRatio_PRE)
		{
			bLockedAspectRatio_PRE = bLockedAspectRatio;
			bIsChanged = true;
		}

		//--

		// Changed Mode

		// memorize free layout mode to not be overwritten by other predefined (top,left..) layout positions.
		if (modeLayout != modeLayout_PRE)
		{
			if (modeLayout_PRE == FREE_LAYOUT)
			{
				// store
				rect_Box_PRE = rect_Box;
			}
			else
			{
				// restore
				rect_Box.set(rect_Box_PRE);
			}

			//-

			modeLayout_PRE = modeLayout;
			bIsChanged = true;

			// workflow
			if (modeLayout != FREE_LAYOUT)
			{
				setEdit(false);
			}
		}

		//--

		updateDoubleClicker();

		//--

		int _w = ofGetWidth();
		int _h = ofGetHeight();

		int _ww = rect_Box.getWidth();
		int _hh = rect_Box.getHeight();

		int _xx;
		int _yy;

		//-

		if (modeLayout == FREE_LAYOUT) {

			_xx = rect_Box.getX();
			_yy = rect_Box.getY();
		}
		else
		{
			//-

			// Top 

			if (modeLayout == TOP_LEFT) {
				_xx = xleft;
				_yy = ytop;
			}
			else if (modeLayout == TOP_CENTER) {
				_xx = xcenter;
				_yy = ytop;
			}
			else if (modeLayout == TOP_RIGHT) {
				_xx = xright;
				_yy = ytop;
			}

			//-

			// Center 

			else if (modeLayout == CENTER) {

				_xx = xcenter;
				_yy = ycenter;
			}

			//-

			// Bottom

			else if (modeLayout == BOTTOM_LEFT) {
				_xx = xleft;
				_yy = ybottom;
			}
			else if (modeLayout == BOTTOM_CENTER) {
				_xx = xcenter;
				_yy = ybottom;
			}
			else if (modeLayout == BOTTOM_RIGHT) {
				_xx = xright;
				_yy = ybottom;
			}

			//--

			// Set

			rect_Box.setX(_xx);
			rect_Box.setY(_yy);
		}

		//-

		if (modeLayout == FREE_LAYOUT)
		{
			if (rect_Box.isEditing())
			{
				rect_Box.draw();
			}
		}

		//--

		// Fit Marks
		xcenter = _w / 2 - _ww / 2 + _padx / 2;
		ycenter = _h / 2 - _hh / 2;
		xleft = _padx;
		xright = _w - _ww - _padx;
		ytop = _pady;
		ybottom = _h - _hh - _pady;

		//--

		// Move clicker linked to the box
		doubleClicker.set(_xx, _yy, _ww, _hh);

		//--

		if (bUseBorder) drawBorder();

		//--

		// Force fit box inside the window
		//bool bContraints = true;
		//if (bContraints) doForceFitOnWindow();
		doForceFitOnWindow();

		ofPopStyle();
	}

	//--------------------------------------------------------------
	void doForceFitOnWindow()
	{
		if (rect_Box.getY() > ybottom) // bottom
		{
			rect_Box.setY(ybottom);
		}
		else if (rect_Box.getX() < xleft) // left
		{
			rect_Box.setX(xleft);
		}
		else if (rect_Box.getX() > xright) // right
		{
			rect_Box.setX(xright);
		}
		else if (rect_Box.getY() < ytop) // top
		{
			rect_Box.setY(ytop);
		}
	};

	//-

private:

	//--------------------------------------------------------------
	void updateDoubleClicker()
	{
		//TODO: too tricky maybe. can be simplified.
		// some drawing are not requiring when not debugging!

		//--

		// 1. Double click swap edit mode

		//-

		// Allow edit only on free layout mode:

#ifdef LOCK_EDIT_ON_NON_FREE_LAYOUT_MODE
		if (modeLayout == FREE_LAYOUT)
#endif
		{
			if (doubleClicker.isMouseDoubleClick())
			{
				bStateEdit = !bStateEdit;

				setEdit(bStateEdit);

				// workflow

				if (bStateEdit)
				{
					if (modeLayout != FREE_LAYOUT) modeLayout = FREE_LAYOUT;
				}

				//modeLayout = FREE_LAYOUT;
			}
	}

		//--

		// 2. Triple clicks swap modeLayout mode

		if (doubleClicker.isMouseTripleClick())
		{
			bState2 = !bState2;

			int i = modeLayout;
			i++;
			if (i >= NUM_LAYOUTS) { modeLayout = FREE_LAYOUT; }
			else { modeLayout = BOX_LAYOUT(i); }
		}

		//--

		doubleClicker.draw();
}

public:

	//--------------------------------------------------------------
	void setToggleEdit()
	{
		setEdit(!rect_Box.isEditing());
	}

	//--------------------------------------------------------------
	void setEdit(bool edit)
	{
		bIsEditing = edit;

		if (bEdit != bIsEditing) bEdit = bIsEditing;

		if (bEdit)
		{
			rect_Box.enableEdit();

			// workflow
			if (modeLayout != FREE_LAYOUT)
			{
				modeLayout = FREE_LAYOUT;
			}
		}
		else
		{
			rect_Box.disableEdit();

			// Save
			rect_Box.saveSettings(path_RectHelpBox, path_Global + "/", false);
		}

		bIsChanged = true;
	}

	////--------------------------------------------------------------
	//void setToggleTheme() {
	//	bThemeDarkOrLight = !bThemeDarkOrLight;
	//	setTheme(bThemeDarkOrLight);
	//}

	////--------------------------------------------------------------
	//void setTheme(bool bTheme) {
	//	bThemeDarkOrLight = bTheme;
	//	// Light theme (false = light)
	//	if (!bThemeDarkOrLight)
	//	{
	//		//_colorText = ofColor(0, 255);
	//		_colorShadow = ofColor(255, 64);
	//		_colorBg = ofColor(225, 64);
	//	}
	//	// Dark theme (white lines & black bg) (true = dark)
	//	else
	//	{
	//		//_colorText = ofColor(255, 150);
	//		_colorShadow = ofColor(16, 225);
	//		_colorBg = ofColor(0, 200);
	//	}
	//}

	//--------------------------------------------------------------
	void setToggleMode() {
		int i = BOX_LAYOUT(modeLayout);
		i++;
		i = i % NUM_LAYOUTS;
		modeLayout = BOX_LAYOUT(i);
	}

	//--------------------------------------------------------------
	void setMode(BOX_LAYOUT mode) {
		modeLayout = mode;
	}

	//--------------------------------------------------------------
	void setLocked(bool b) {
		bLocked = b;
		if (b) doubleClicker.disableAllEvents();
		else doubleClicker.enableAllEvents();
	}
	//--------------------------------------------------------------
	void setToggleLocked() {
		bLocked = !bLocked;
		setLocked(bLocked);
	}

private:

	bool isLocked() { return bLocked; }

	//--

public:

	//--------------------------------------------------------------
	void setVisible(bool b) {
		bGui = b;
	}
	//--------------------------------------------------------------
	void setToggleVisible() {
		bGui = !bGui;
	}

	//--

	//--------------------------------------------------------------
	void setLockAspectRatio(bool b) {
		bLockedAspectRatio = b;
		rect_Box.setLockAspectRatio(bLockedAspectRatio);
	}

	//--------------------------------------------------------------
	void setToggleLockAspectRatio() {
		bLockedAspectRatio = !bLockedAspectRatio;
		rect_Box.setLockAspectRatio(bLockedAspectRatio);
	}

	//--------------------------------------------------------------
	bool isLockedAspectRatio() {
		return bLockedAspectRatio;
	}

};

//if (key == 'T') boxWidget.setToggleLocked();
//helpInfo += "LOCKED CLICKS " + (string)(boxWidget.isLocked() ? "TRUE" : "FALSE");

//--
