
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

	BUG: double clicked do not enables edit mode??

	+ save mode picked too
	+ fit screen or mini/ big modes to use on a video player.

*/


//-


class ofxSurfingBoxInteractive /* : public ofBaseApp*/
{
private:

	bool bEnableMouseWheel = true;

public:

	void setEnableMouseWheel(bool b) { bEnableMouseWheel = b; }

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
		
		bEdit.removeListener(this, &ofxSurfingBoxInteractive::Changed_Edit);
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
	bool isEditing() {
		
		//bool b = rect_Box.isEditing();
		//bool b = bIsEditing || rect_Box.isEditing();
		bool b = bIsEditing;
		
		return b;
	}

	////--------------------------------------------------------------
	//string getEditingString() {
	//	if (rect_Box.isEditing()) return "EDITING"; else return "NOT EDITING";
	//}

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
			//case 8: str_modeLayout = "LOCKED"; break;
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

	// legacy
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
	void ofxSurfingBoxInteractive::Changed_Edit(bool & edit) {
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
	void draw()
	{
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

		drawDebugDoubleClick();

		//--

		int _w = ofGetWidth();
		int _h = ofGetHeight();

		int _padx = 10;
		int _pady = 10;

		int _xx;
		int _yy;
		int _ww;
		int _hh;

		_ww = rect_Box.getWidth();
		_hh = rect_Box.getHeight();

		//-

		if (modeLayout == FREE_LAYOUT) {

			_xx = rect_Box.getX();
			_yy = rect_Box.getY();
		}
		else
		{
			if (modeLayout == CENTER) {

				_xx = _w / 2 - _ww / 2 - _padx;
				_yy = _h / 2 - _hh / 2 - _pady;
			}
			else if (modeLayout == TOP_CENTER) {

				_xx = _w / 2 - _ww / 2 - _padx;
				_yy = 1 * _pady;
			}
			else if (modeLayout == BOTTOM_CENTER) {

				_xx = _w / 2 - _ww / 2 - _padx;
				_yy = _h - _hh - _pady;
			}

			else if (modeLayout == TOP_LEFT) {

				_xx = _padx;
				_yy = _pady;
			}
			else if (modeLayout == TOP_RIGHT) {

				_xx = _w - _ww - _padx;
				_yy = _pady;
			}
			else if (modeLayout == BOTTOM_LEFT) {

				_xx = _padx;
				_yy = _h - _hh - _pady;
			}
			else if (modeLayout == BOTTOM_RIGHT) {

				_xx = _w - _ww - _padx;
				_yy = _h - _hh - _pady;
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

		//-

		// Force fit box inside the window
		bool bContraints = true;
		if(bContraints)
		{
			float _xmax = _w - _ww - _padx;
			float _ymax = _h - _hh - _pady;
			float _ymin = 2 * _pady;

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

		//--

		// Move clicker linked to the box
		doubleClicker.set(_xx, _yy, _ww, _hh);

		//--

		if(bUseBorder) drawBorder();
	}

	//-

private:

	//--------------------------------------------------------------
	void drawDebugDoubleClick()
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