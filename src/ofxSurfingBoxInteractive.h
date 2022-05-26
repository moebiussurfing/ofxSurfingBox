
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

*/


/*

	TODO: 
	
	+ save mode picked too
	+ fit screen or mini/ big modes to use on a video player.

*/


//-


class ofxSurfingBoxInteractive /* : public ofBaseApp*/
{

public:

	ofParameter<bool> bGui{ "Box Rectangle", true };//exposed toggle to be used or linked in other parent scope guis!

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
		//rect_Box.saveSettings(path_RectHelpBox, path_Global + "/" + path_Name + "/", false);
	}

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
		//LOCKED,

		NUM_LAYOUTS
	};

private:

	DoubleClicker doubleClicker;

	BOX_LAYOUT modeLayout = FREE_LAYOUT;
	string str_modeLayout = "";

	bool bLocked = false;
	bool bLockedAspectRatio = false;
	bool bIsChanged = false;

public:

	// a simple callback to trig when theme or layout changed
	//--------------------------------------------------------------
	bool isChanged() {
		if (!bIsChanged) return false;
		else {
			bIsChanged = false;
			return true;
		}
	}

	//--------------------------------------------------------------
	string getEditing() {
		if (rect_Box.isEditing()) return "EDITING"; else return "NOT EDITING";
	}

	//--------------------------------------------------------------
	string getMode() {
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
			//case 8: str_modeLayout = "LOCKED"; break;
		default: str_modeLayout = "UNKNOWN LAYOUT"; break;
		}
		return str_modeLayout;
	}

	ofxInteractiveRect rect_Box = { "_" };

	ofRectangle getRectangle() {
		return rect_Box;
	}

private:

	std::string path_RectHelpBox = "_myBox";
	std::string path_Global = "ofxSurfingBoxInteractive/";// can be setted before setup
	//std::string path_Name = "appSettings";// subfolder for app session settings

	ofColor _colorButton;// bg selected button
	ofColor _colorBg;// background color
	ofColor _colorShadow;
	bool _bUseShadow;

	bool bCenter = true;
	bool bLeftPosition = false;

	bool bState1 = false;
	bool bState2 = false;

	float round = 5;
	int marginBorders = 50;

public:

	////ofParameter<glm::vec2> shape{ "Shape", glm::vec2(1920 / 2, 1080 / 2), glm::vec2(0,0), glm::vec2(19200,1080) };
	
	//--------------------------------------------------------------
	void setShape(ofRectangle shape) {
		rect_Box.setRect(shape.x, shape.y, shape.getWidth(), shape.getHeight());
	}

public:

	//--------------------------------------------------------------
	void setPath(string path) {//call before setup. Will set path to save settings into.
		path_Global = path;
	}

	//--------------------------------------------------------------
	void reset() {

		int sz = 200;
		rect_Box.setPosition(ofGetWidth() / 2 - (sz / 2), ofGetHeight() / 2 - (sz / 2));
		rect_Box.setWidth(sz);
		rect_Box.setHeight(sz);
	}

	//--------------------------------------------------------------
	void setup() {

		//_bUseShadow = true;

		//setTheme(bThemeDarkOrLight);

		doubleClicker.set(0, 0, ofGetWidth(), ofGetHeight()); // default full screen
		doubleClicker.setDebug(false);

		// Default position
		reset();

		//----

		ofxSurfingHelpers::CheckFolder(path_Global + "/");
		//ofxSurfingHelpers::CheckFolder(path_Global + "/" + path_Name + "/");

		// Load settings
		rect_Box.loadSettings(path_RectHelpBox, path_Global + "/", false);
		//rect_Box.loadSettings(path_RectHelpBox, path_Global + "/" + path_Name + "/", false);

		////--
		//// 
		//// We dont need draggable borders and decoration.
		//rect_Box.setLockResize(true);
		////rect_Box.setLockResize(!bNoText);
		//rect_Box.setTransparent();
	}

	//--------------------------------------------------------------
	void draw()
	{
		static ofRectangle rect_Box_PRE = rect_Box;

		//--

		// Simple callbacks
		static BOX_LAYOUT modeLayout_PRE = NUM_LAYOUTS;
		static bool bLockedAspectRatio_PRE = false;

		if (bLockedAspectRatio != bLockedAspectRatio_PRE) {
			bLockedAspectRatio_PRE = bLockedAspectRatio;
			bIsChanged = true;
		}

		//--

		// Changed Mode

		if (modeLayout != modeLayout_PRE)
		{
			if (modeLayout_PRE == FREE_LAYOUT)
			{
				//store
				rect_Box_PRE = rect_Box;
			}
			else
			{
				//restore
				rect_Box.set(rect_Box_PRE);
			}

			//-

			modeLayout_PRE = modeLayout;
			bIsChanged = true;

			// workflow
			if (modeLayout != FREE_LAYOUT) {
				setEdit(false);
			}
		}

		//--

		//if (!bGui) return;

		ofPushStyle();

		//TODO: not drawing..
		drawDoubleClickDebug();

		//-

		float _w = ofGetWidth();
		float _h = ofGetHeight();

		int _padx = 10;
		int _pady = 10;

		int _xx;
		int _yy;

		float _ww;
		float _hh;

		_ww = rect_Box.getWidth();
		_hh = rect_Box.getHeight();

		//-

		if (modeLayout == FREE_LAYOUT) {

			_xx = rect_Box.getX();
			_yy = rect_Box.getY();
		}
		else
		{
			if (modeLayout == BOTTOM_CENTER) {

				_xx = _w / 2 - _ww / 2 - _padx;
				_yy = _h - _hh - _pady;
			}
			else if (modeLayout == BOTTOM_LEFT) {

				_xx = _padx;
				_yy = _h - _hh - _pady;
			}
			else if (modeLayout == BOTTOM_RIGHT) {

				_xx = _w - _ww - _padx;
				_yy = _h - _hh - _pady;
			}

			else if (modeLayout == TOP_CENTER) {

				_xx = _w / 2 - _ww / 2 - _padx;
				_yy = 2 * _pady;
			}
			else if (modeLayout == TOP_LEFT) {

				_xx = _padx;
				_yy = 2 * _pady;
			}
			else if (modeLayout == TOP_RIGHT) {

				_xx = _w - _ww - _padx;
				_yy = 2 * _pady;
			}

			else if (modeLayout == CENTER) {

				_xx = _w / 2 - _ww / 2 - _padx;
				_yy = _h / 2 - _hh / 2 - _pady;
			}

			rect_Box.setX(_xx);
			rect_Box.setY(_yy);
		}

		//-

		ofColor colorBg;

		if (modeLayout == FREE_LAYOUT)
		{
			if (rect_Box.isEditing())
			{
				float a = ofxSurfingHelpers::getFadeBlink(0.6f, 1.f);

				ofColor c = ofColor(_colorBg, _colorBg.a * a);
				rect_Box.draw();

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

		//-

		ofPopStyle();

		//-

		// Force fit box inside the window
		{
			float _xmax = _w - _ww - _padx;
			float _ymax = _h - _hh - _pady;
			float _ymin = 3 * _pady;

			if (rect_Box.getY() > _ymax)//bottom
			{
				rect_Box.setY(_ymax);
			}
			else if (rect_Box.getX() < _padx)//left
			{
				rect_Box.setX(_padx);
			}
			else if (rect_Box.getX() > _xmax)//right
			{
				rect_Box.setX(_xmax);
			}
			else if (rect_Box.getY() < _ymin)//top
			{
				rect_Box.setY(_ymin);
			}
		}

		doubleClicker.set(_xx, _yy, _ww, _hh);
	}

	//-

private:

	//--------------------------------------------------------------
	void drawDoubleClickDebug()
	{
		//--

		// 1. Double click swap edit mode

		//-

		// Allow edit only on free layout mode:
#ifdef LOCK_EDIT_ON_NON_FREE_LAYOUT_MODE
		if (modeLayout == FREE_LAYOUT)
#endif
			//-

		{
			if (doubleClicker.isMouseDoubleClick())
				//if (doubleClicker.isMouseTripleClick()) 
			{
				bState1 = !bState1;

				setEdit(bState1);

				// workflow
				if (bState1)
				{
					if (modeLayout != FREE_LAYOUT) modeLayout = FREE_LAYOUT;
				}
				//modeLayout = FREE_LAYOUT;
			}
		}

		//--

		// 2. Triple clicks swap modeLayout mode

		//if (doubleClicker.isMouseDoubleClick())
		if (doubleClicker.isMouseTripleClick())
		{
			bState2 = !bState2;

			int i = modeLayout;
			i++;
			if (i >= NUM_LAYOUTS) { modeLayout = FREE_LAYOUT; }
			else { modeLayout = BOX_LAYOUT(i); }
		}


		doubleClicker.draw();
	}

public:

	//--------------------------------------------------------------
	void setToggleEdit()
	{
		setEdit(!rect_Box.isEditing());
		//rect_Box.toggleEdit();
	}

	//--------------------------------------------------------------
	void setEdit(bool bEdit)
	{
		if (bEdit)
		{
			rect_Box.enableEdit();

			//workflow
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
			//rect_Box.saveSettings(path_RectHelpBox, path_Global + "/" + path_Name + "/", false);
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
	//		_colorButton = ofColor(255, 64);
	//		_colorBg = ofColor(225, 64);
	//	}
	//	// Dark theme (white lines & black bg) (true = dark)
	//	else
	//	{
	//		//_colorText = ofColor(255, 150);
	//		_colorButton = ofColor(16, 225);
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