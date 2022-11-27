
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

	+ add disable save settings
		organize path better
	+ add click + space to easy edit/lock
	+ add all settings -> get from text box
	+ fit screen or mini/ big modes to use on a video player.

*/

/*
		// constraint sizes
		glm::vec2 shapeMin(200, 400);
		boxWidgets.setRectConstraintMin(shapeMin);
		glm::vec2 shapeMax(ofGetWidth(), ofGetHeight());
		boxWidgets.setRectConstraintMax(shapeMax);
*/


//--


class ofxSurfingBoxInteractive /* : public ofBaseApp*/
{

public:

	//--------------------------------------------------------------
	ofxSurfingBoxInteractive::ofxSurfingBoxInteractive()
	{
		//setName("myBox");
	}

	//--------------------------------------------------------------
	ofxSurfingBoxInteractive::~ofxSurfingBoxInteractive()
	{
		// All app settings
		ofxSurfingHelpers::CheckFolder(path_Global + "/");
		rect_Box.saveSettings(path_RectHelpBox, path_Global + "/", false);

		bEdit.removeListener(this, &ofxSurfingBoxInteractive::Changed_Edit);
		bUseBorder.removeListener(this, &ofxSurfingBoxInteractive::Changed_Border);
		bGui.removeListener(this, &ofxSurfingBoxInteractive::Changed_bGui);

		ofxSurfingHelpers::saveGroup(params_AppSession, path_Global + "/" + path_AppSession);
	}

	ofParameter<bool> bGui{ "Box Rectangle", true };
	//exposed toggle to be used or linked in other parent scope guis!

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

public:

	// Lock drag borders
	void setLockX(bool b) { rect_Box.setLockX(b); };//set enable/disable drag x interaction
	void setLockY(bool b) { rect_Box.setLockY(b); };//set enable/disable drag y interaction
	void setLockW(bool b) { rect_Box.setLockW(b); };//set enable/disable drag width interaction
	void setLockH(bool b) { rect_Box.setLockH(b); };//set enable/disable drag height interaction

private:

	bool bDebugDoubleClick = false;

	ofParameterGroup params_AppSession{ "Settings" };


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

	// callback to re fresh fbo's
	//--------------------------------------------------------------
	bool isChangedShape() {
		static float w;
		static float h;

		bool changed = false;

		if (w != this->getWidth() || h != this->getHeight())
		{
			changed = true;

			w = this->getWidth();
			h = this->getHeight();
		}

		return changed;
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

	bool isVisible() { return bGui.get(); }

	//--------------------------------------------------------------
	ofRectangle getRectangle() {
		return rect_Box;
	}
	float getX() { return rect_Box.x; }
	float getY() { return rect_Box.y; }
	float getWidth() { return rect_Box.getWidth(); }
	float getHeight() { return rect_Box.getHeight(); }

	void setUseBorder(bool b) { bUseBorder = b; }

	//--------------------------------------------------------------
	void setUseBorderBlinking(bool b) {
		bUseBorderBlinking = b;
		if (b) bUseBorder = true;
	}
	//--------------------------------------------------------------
	void setToggleUseBorder() {
		bUseBorder = !bUseBorder;
	}
	//--------------------------------------------------------------
	void setToggleUseBorderBlinking() {
		bUseBorderBlinking = !bUseBorderBlinking;
		if (bUseBorderBlinking) bUseBorder = true;
	}

	void setBorderColor(ofColor c) { _colorBorder = c; }

	void setWokflow(bool b) { bWorflow = b; } // enables some automated workflow. ex: disable gui edit when hidde.


private:

	//bool bUseBorder = false;
	//bool bUseBorder = true;

	std::string path_RectHelpBox = "myBox";
	std::string path_Global = "ofxSurfingBoxInteractive/"; // can be settled before setup
	//std::string path_Name = "appSettings"; // sub folder for app session settings
	std::string suffixSettings = "Settings.xml";
	std::string path_AppSession = path_RectHelpBox + "_" + suffixSettings;

	ofColor _colorButton;// bg selected button
	ofColor _colorBg;// background color
	ofColor _colorShadow;
	ofColor _colorBorder;
	bool _bUseShadow;

	bool bWorflow = false;

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
	ofParameter<bool> bTransparent;
	ofParameter<bool> bUseBorder;
	ofParameter<bool> bUseBorderBlinking;

	//--------------------------------------------------------------
	void setName(string name) {
		path_RectHelpBox = name;
		bGui.setName(name);
		path_AppSession = name + "_" + suffixSettings;
	}

	//--

private:

	//ofRectangle shapeConstraintMin;//min shape
	//ofRectangle shapeConstraintMax;//max shape

public:

	//--------------------------------------------------------------
	void setRectConstraintMin(glm::vec2 shape) {
		rect_Box.setRectConstraintMin(shape);
	}
	//--------------------------------------------------------------
	void setRectConstraintMax(glm::vec2 shape) {
		rect_Box.setRectConstraintMax(shape);
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
	void setX(float x) {
		rect_Box.setX(x);
	}
	//--------------------------------------------------------------
	void setY(float y) {
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
	void reset(bool bOnlySize = false, int width = 320) {

		int sz = width;
		rect_Box.setWidth(sz);
		rect_Box.setHeight(sz);
		if (!bOnlySize) rect_Box.setPosition(ofGetWidth() / 2 - (sz / 2), ofGetHeight() / 2 - (sz / 2));
	}

	//--------------------------------------------------------------
	void setup()
	{
		bEdit.set("Edit", false);
		bUseBorder.set("Border", false);
		bUseBorderBlinking.set("Blink", false);
		bTransparent.set("Transparent", false);

		bEdit.addListener(this, &ofxSurfingBoxInteractive::Changed_Edit);
		bUseBorder.addListener(this, &ofxSurfingBoxInteractive::Changed_Border);
		bGui.addListener(this, &ofxSurfingBoxInteractive::Changed_bGui);

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

		// extra settings
		params_AppSession.add(bEdit);
		params_AppSession.add(bTransparent);
		params_AppSession.add(bUseBorder);
		params_AppSession.add(bUseBorderBlinking);

		ofxSurfingHelpers::loadGroup(params_AppSession, path_Global + "/" + path_AppSession);

		//--

		//// We don't need draggable borders and decoration.
		//rect_Box.setLockResize(true);
		////rect_Box.setLockResize(!bNoText);
		//rect_Box.setTransparent();

		//rect_Box.setRectConstraintMin
	}

	//--------------------------------------------------------------
	void Changed_Edit(bool& edit) {
		ofLogNotice("ofxSurfingBoxInteractive") << (__FUNCTION__) << "Edit : " << edit;
		setEdit(edit);
	}
	//--------------------------------------------------------------
	void Changed_Border(bool& b) {
		ofLogNotice("ofxSurfingBoxInteractive") << (__FUNCTION__) << "Border: " << b;
		setUseBorder(b);
	}

	//--------------------------------------------------------------
	void Changed_bGui(bool& b)
	{
		ofLogNotice("ofxSurfingBoxInteractive") << (__FUNCTION__) << "bGui: " << b;

		//workflow
		if (b)
		{
			//rect_Box.enableEdit();
			doubleClicker.enableAllEvents();
		}

		else {
			//workflow
			if (bWorflow) {
				rect_Box.disableEdit();
				doubleClicker.disableAllEvents();
			}
		}
	}

	//--------------------------------------------------------------
	//void drawBorder(ofColor color = ofColor(0, 255))
	void drawBorder()
	{
		// Border
		ofPushStyle();
		ofNoFill();
		ofSetColor(_colorBorder);
		ofSetLineWidth(2.0);
		ofDrawRectangle(getRectangle());
		ofPopStyle();
	}

	//--------------------------------------------------------------
	//void drawBorderBlinking(ofColor color = ofColor(255, 200))
	void drawBorderBlinking()
	{
		if (!bGui) return;//TODO:

		int a = ofMap(ofxSurfingHelpers::Bounce(), 0, 1, 24, 64);
		ofColor c = ofColor(_colorBorder, a);

		//drawBorder(c);
		// Border
		ofPushStyle();
		ofNoFill();
		ofSetColor(c);
		ofSetLineWidth(2.0);
		ofDrawRectangle(getRectangle());
		ofPopStyle();
	}

	//--------------------------------------------------------------
	void draw()
	{
		if (!bGui) return;//TODO:

		ofPushStyle();
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

			if (this->isEditing())
			{
				drawBorderBlinking();
			}
			else
			{
				if (!bTransparent)
				{
					if (bUseBorder)
					{
						if (bUseBorderBlinking) drawBorderBlinking();
						else drawBorder();
					}
				}
			}

			//--

			// Force fit box inside the window
			//bool bContraints = true;
			//if (bContraints) doForceFitOnWindow();
			doForceFitOnWindow();

			//if (bUseBorder) this->drawBorder();
			////if (bUseBorder) this->drawBorderBlinking();
		}
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

		// 2. Left pressed + right click : close box!
		if (this->isEditing())
			if (ofGetMousePressed(0) && doubleClicker.isMouseRightClick()) {
				//if (doubleClicker.isMouseRightPressedThenPressedLeft()) {
				ofLogWarning("TextBoxWidget") << (__FUNCTION__);
				bGui = false;
			}

		//--

		// 3. Right click swap modeLayout mode
		if (doubleClicker.isMouseRightClick())
			// 3. Triple clicks swap modeLayout mode
			//if (doubleClicker.isMouseTripleClick())
		{
			bState2 = !bState2;

			int i = modeLayout;
			i++;
			if (i >= NUM_LAYOUTS) { modeLayout = FREE_LAYOUT; }
			else { modeLayout = BOX_LAYOUT(i); }

			bIsChanged = true;
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
	void setLocked(bool b) {//enable/disable move and resize and all interaction
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
