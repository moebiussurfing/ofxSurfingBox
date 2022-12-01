
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

	+ fix loading well
	+ store layout mode, not only the free layout
	+ update text box from this class
	+ fit screen or mini / big modes to use on a video player.

*/

#define AUTO_CASE_CREATE(a) case a: return #a

//--


class ofxSurfingBoxInteractive
{
public:

	//--------------------------------------------------------------
    ofxSurfingBoxInteractive()
	{
		setRectConstraintMin(glm::vec2(50, 50));
		setPads(4, 4);
	}

	//--------------------------------------------------------------
    ~ofxSurfingBoxInteractive()
	{
		bEdit.removeListener(this, &ofxSurfingBoxInteractive::Changed_Edit);
		bUseBorder.removeListener(this, &ofxSurfingBoxInteractive::Changed_Border);
		bGui.removeListener(this, &ofxSurfingBoxInteractive::Changed_bGui);

		// Settings

		ofxSurfingHelpers::CheckFolder(path_Global + "/");

		// Restore the one from free layout mode before save
		if (modeLayout != FREE_LAYOUT) rect_Box.set(rect_Box_STORE);

		rect_Box.saveSettings(path_RectHelpBox, path_Global + "/", false);

		iModeLayout = (int)modeLayout;//read value before save

		ofxSurfingHelpers::saveGroup(params_AppSession, path_Global + "/" + path_AppSession);
	}

	//--------------------------------------------------------------
	void setup()
	{
		bEdit.set("Edit", false);
		bUseBorder.set("Border", false);
		bUseBorderBlinking.set("Blink", false);
		bTransparent.set("Transparent", false);
		iModeLayout.set("ModeLayout", 0, 0, NUM_LAYOUTS - 1);

		bEdit.addListener(this, &ofxSurfingBoxInteractive::Changed_Edit);
		bUseBorder.addListener(this, &ofxSurfingBoxInteractive::Changed_Border);
		bGui.addListener(this, &ofxSurfingBoxInteractive::Changed_bGui);

		doubleClicker.set(0, 0, ofGetWidth(), ofGetHeight()); // default full screen

		doubleClicker.setDebug(bDebugDoubleClick);

		// Default position
		reset(false);

		//--

		ofxSurfingHelpers::CheckFolder(path_Global + "/");

		// Load settings
		rect_Box.loadSettings(path_RectHelpBox, path_Global + "/", false);

		// Store
		rect_Box_STORE = rect_Box.getRect();

		rect_Box.setEnableMouseWheel(true);
		rect_Box.setAutoSave(false);

		//--

		// Extra settings
		params_AppSession.add(bEdit);
		params_AppSession.add(iModeLayout);
		params_AppSession.add(bTransparent);
		params_AppSession.add(bUseBorder);
		params_AppSession.add(bUseBorderBlinking);

		ofxSurfingHelpers::loadGroup(params_AppSession, path_Global + "/" + path_AppSession);

		modeLayout = BOX_LAYOUT(iModeLayout.get());
	}

	//--------------------------------------------------------------
	void draw()
	{
		if (!bGui) return;

		ofPushStyle();
		{
			//--

			// Simple callbacks

			static BOX_LAYOUT modeLayout_PRE = NUM_LAYOUTS;
			static bool bLockedAspectRatio_PRE = !bLockedAspectRatio;

			//--

			//TODO:
			// Check if rectangle changed
			// Pos / Size 
			if (rect_Box.getRect() != rect_Box_PRE)
			{
				rect_Box_PRE = rect_Box.getRect();
				bIsChanged = true;
			}

			//--

			// Changed Mode

			// Memorize free layout mode 
			// to not be overwritten by 
			// other predefined (top,left..) layout positions.
			if (modeLayout != modeLayout_PRE)
			{
				str_modeLayout = getModeLayoutString();

				// If we are leaving free mode, 
				// we must remember the rectangle@
				if (modeLayout_PRE == FREE_LAYOUT)
				{
					// Store
					rect_Box_STORE = rect_Box.getRect();
				}
				// Changed to free layout
				else if (modeLayout == FREE_LAYOUT)
				{
					// Restore
					rect_Box.set(rect_Box_STORE);
				}

				//--

				// flag
				modeLayout_PRE = modeLayout;
				bIsChanged = true;

				// workflow
				if (modeLayout != FREE_LAYOUT)
				{
					setEdit(false);
				}
			}

			//--

			// Calculate important point to define layout

			// window
			float _w = ofGetWidth();
			float _h = ofGetHeight();

			// box size
			float _ww = rect_Box.getWidth();
			float _hh = rect_Box.getHeight();

			// box pos
			float _xx = 0;
			float _yy = 0;

			//--

			if (modeLayout == FREE_LAYOUT)
			{
				_xx = rect_Box.getX();
				_yy = rect_Box.getY();
			}
			else
			{
				//--

				// Top 

				if (modeLayout == TOP_LEFT)
				{
					_xx = xleft;
					_yy = ytop;
				}
				else if (modeLayout == TOP_CENTER)
				{
					_xx = xcenter;
					_yy = ytop;
				}
				else if (modeLayout == TOP_RIGHT)
				{
					_xx = xright;
					_yy = ytop;
				}

				//--

				// Center 

				else if (modeLayout == CENTER_LEFT)
				{
					_xx = xleft;
					_yy = ycenter;
				}
				else if (modeLayout == CENTER_CENTER)
				{
					_xx = xcenter;
					_yy = ycenter;
				}
				else if (modeLayout == CENTER_RIGHT)
				{
					_xx = xright;
					_yy = ycenter;
				}

				//--

				// Bottom

				else if (modeLayout == BOTTOM_LEFT)
				{
					_xx = xleft;
					_yy = ybottom;
				}
				else if (modeLayout == BOTTOM_CENTER)
				{
					_xx = xcenter;
					_yy = ybottom;
				}
				else if (modeLayout == BOTTOM_RIGHT)
				{
					_xx = xright;
					_yy = ybottom;
				}

				//--

				// Set

				rect_Box.setX(_xx);
				rect_Box.setY(_yy);
			}

			//--

			// Calculate Fit Marks
			{
				xcenter = _w / 2.f - _ww / 2.f;
				ycenter = _h / 2.f - _hh / 2.f;

				xleft = xpad;
				xright = _w - _ww - xpad;

				ytop = ypad;
				ybottom = _h - _hh - ypad;
			}

			//--

			// Force fit box inside the window
			if (bForceFitInsideWindow) doForceFitOnWindow();

			//----

			// Move clicker linked to the box
			doubleClicker.set(_xx, _yy, _ww, _hh);

			// Double clicker handler
			drawDoubleClicker();

			//----

			// Aspect changed
			if (bLockedAspectRatio != bLockedAspectRatio_PRE)
			{
				bLockedAspectRatio_PRE = bLockedAspectRatio;
				bIsChanged = true;
			}

			//--

			// Draw

			if (modeLayout == FREE_LAYOUT)
			{
				if (rect_Box.isEditing())
				{
					rect_Box.draw();
				}
			}

			//----

			// Draw border

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

			//----

			// Debug
			// Detect changes
			// types and change colors.
			if (bDebug) drawDebug();
		}
		ofPopStyle();
	}

	//--

private:

	ofxInteractiveRect rect_Box = { "_" };

	//--

public:

	// Exposed visible toggle to be used or linked in other parent scope GUIs!
	ofParameter<bool> bGui{ "Box", true };

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

	//--

	BOX_LAYOUT modeLayout = FREE_LAYOUT;
	string str_modeLayout = "";

	ofRectangle rect_Box_PRE;
	ofRectangle rect_Box_STORE;

	//--

public:

	ofParameter<bool> bEdit;
	ofParameter<bool> bTransparent;
	ofParameter<bool> bUseBorder;
	ofParameter<bool> bUseBorderBlinking;
	ofParameter<int> iModeLayout;//only used to load/store settings

private:

	std::string path_RectHelpBox = "myBox"; // default name
	std::string path_Global = "ofxSurfingBoxInteractive/"; // default path
	std::string suffixSettings = "Settings.xml";
	std::string path_AppSession = path_RectHelpBox + "_" + suffixSettings;

	ofParameterGroup params_AppSession{ "Settings" };

	ofColor _colorButton;// bg selected button
	ofColor _colorBg;// background color
	ofColor _colorShadow;
	ofColor _colorBorder;

	//bool bWorflow = true;

	bool bIsEditing = false;
	bool bIsChanged = false;

	bool bLocked = false;
	bool bLockedAspectRatio = false;

	bool bForceFitInsideWindow = true;
	bool bDebug = false;

private:

	// Layout helper marks
	// Fit inside window marks
	float xpad = 0;
	float ypad = 0;
	float xcenter;
	float ycenter;
	float xleft;
	float xright;
	float ytop;
	float ybottom;

	//--

	// DoubleClick engine

private:

	DoubleClicker doubleClicker;
	bool bStateEdit = false;
	bool bState2 = false;

	//--

public:

	// Lock drag borders
	//--------------------------------------------------------------
	void setLockX(bool b) { rect_Box.setLockX(b); };//set enable/disable drag x interaction
	//--------------------------------------------------------------
	void setLockY(bool b) { rect_Box.setLockY(b); };//set enable/disable drag y interaction
	//--------------------------------------------------------------
	void setLockW(bool b) { rect_Box.setLockW(b); };//set enable/disable drag width interaction
	//--------------------------------------------------------------
	void setLockH(bool b) { rect_Box.setLockH(b); };//set enable/disable drag height interaction

public:

	// Debug DoubleClick

	bool bDebugDoubleClick = false;
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
	bool isChanged()
	{
		if (!bIsChanged) return false;
		else
		{
			bIsChanged = false;
			return true;
		}
	}

	// Callback useful i.e to re fresh Fbo's / viewports.
	//--------------------------------------------------------------
	bool isChangedShape()
	{
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

	//--

public:

	//--------------------------------------------------------------
	void setDebug(bool b) {
		bDebug = b;
		bUseBorder = b;
		//setEdit(true);
	};
	//--------------------------------------------------------------
	void setToggleDebug() {
		bDebug = !bDebug;
		bUseBorder = bDebug;
	}

	//--

	//--------------------------------------------------------------
	bool isVisible() const { return bGui.get(); }

	//--------------------------------------------------------------
	ofRectangle getRectangle() {
		return rect_Box;
	}

	//--------------------------------------------------------------
	float getX() { return rect_Box.x; }
	//--------------------------------------------------------------
	float getY() { return rect_Box.y; }
	//--------------------------------------------------------------
	float getWidth() { return rect_Box.getWidth(); }
	//--------------------------------------------------------------
	float getHeight() { return rect_Box.getHeight(); }

	//--------------------------------------------------------------
	void setUseBorder(bool b) { bUseBorder = b; }

	//--------------------------------------------------------------
	void setPads(float x, float y) {//call after setup
		xpad = x;
		ypad = y;
		rect_Box.setPads(xpad, ypad);
	}

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

	//--------------------------------------------------------------
	void setBorderColor(ofColor c) { _colorBorder = c; }

	//--------------------------------------------------------------
	//void setWokflow(bool b) { bWorflow = b; } // enables some automated workflow. ex: disable gui edit when hide.

	//--------------------------------------------------------------
	void setName(string name) {
		path_RectHelpBox = name;
		bGui.setName(name);
		path_AppSession = name + "_" + suffixSettings;
	}

	//--

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

	//--------------------------------------------------------------
	void setPathGlobal(string path) {//call before setup. Will set path to save settings into.
		path_Global = path;
	}
	// Legacy
	//--------------------------------------------------------------
	void setPath(string path) {//call before setup. Will set path to save settings into.
		path_Global = path;
	}

	//--------------------------------------------------------------
	void reset(bool bOnlySize = false, int width = 400) {

		float sz = width;
		rect_Box.setWidth(sz);
		rect_Box.setHeight(sz);
		if (!bOnlySize) rect_Box.setPosition(ofGetWidth() / 2.f - (sz / 2.f), ofGetHeight() / 2.f - (sz / 2.f));
	}

	//--------------------------------------------------------------
	void Changed_Edit(bool& edit) {
		ofLogNotice("ofxSurfingBoxInteractive") << " " << (__FUNCTION__) << "Edit : " << edit;
		setEdit(edit);
	}
	//--------------------------------------------------------------
	void Changed_Border(bool& b) {
		ofLogNotice("ofxSurfingBoxInteractive") << " " << (__FUNCTION__) << "Border: " << b;
		setUseBorder(b);
	}

	//--------------------------------------------------------------
	void Changed_bGui(bool& b)
	{
		ofLogNotice("ofxSurfingBoxInteractive") << " " << (__FUNCTION__) << "bGui: " << b;

		//workflow
		if (b)
		{
			doubleClicker.enableAllEvents();
		}
		else //workflow
		{
			//if (bWorflow)
			{
				rect_Box.disableEdit();
				doubleClicker.disableAllEvents();
			}
		}
	}

	//--------------------------------------------------------------
	void drawBorder()
	{
		ofPushStyle();
		ofNoFill();
		ofSetColor(_colorBorder);
		ofSetLineWidth(2.0);
		ofDrawRectangle(getRectangle());
		ofPopStyle();
	}

	//--------------------------------------------------------------
	void drawBorderBlinking()
	{
		if (!bGui) return;//TODO:

		int a = ofMap(ofxSurfingHelpers::Bounce(), 0, 1, 24, 64);
		ofColor c = ofColor(_colorBorder, a);

		// Border
		ofPushStyle();
		ofNoFill();
		ofSetColor(c);
		ofSetLineWidth(2.0);
		ofDrawRectangle(getRectangle());
		ofPopStyle();
	}

	//--------------------------------------------------------------
	void drawDebug()
	{
		//auto r = this->getRectangle();
		auto r = rect_Box;

		// box size
		float _ww = r.getWidth();
		float _hh = r.getHeight();

		//--

		ofColor c1, c2;
		static ofRectangle r_;

		bool bChanged = false;
		bool bChangedSz = false;
		bool bChangedPos = false;

		glm::vec2 sz(r.getWidth(), r.getHeight());
		glm::vec2 pos(r.getX(), r.getY());
		static glm::vec2 sz_;
		static glm::vec2 pos_;

		if (r != r_) {
			r_ = r;
			bChanged = true;
		}
		if (sz != sz_) {
			sz_ = sz;
			bChangedSz = true;
		}
		if (pos != pos_) {
			pos_ = pos;
			bChangedPos = true;
		}

		string s1 = "Pos  " + ofToString(r.getX(), 0) + " " + ofToString(r.getY(), 0);
		string s2 = "Sz   " + ofToString(r.getWidth(), 0) + "x" + ofToString(r.getHeight(), 0);
		c1 = 0;
		c2 = 0;
		if (bChangedPos) c1 = ofColor::red;
		if (bChangedSz) c2 = ofColor::green;

		auto p = r.getTopLeft() + glm::vec2(9 + BORDER_DRAG_SIZE, 19 + BORDER_DRAG_SIZE);
		ofDrawBitmapStringHighlight(s1, p, c1, 255);

		p = p + glm::vec2(0, 20);
		ofDrawBitmapStringHighlight(s2, p, c2, 255);

		string s3 = "";
		s3 += "MODE       " + this->getModeLayoutString() + "\n";
		s3 += "EDIT       " + ofToString(this->isEditing() ? "TRUE" : "FALSE") + "\n\n";
		s3 += "xPad       " + ofToString(xpad, 0) + "\n";
		s3 += "yPad       " + ofToString(ypad, 0) + "\n\n";
		s3 += "xLeft      " + ofToString(xleft, 0) + "\n";
		s3 += "yTop       " + ofToString(ytop, 0) + "\n";
		s3 += "xRight     " + ofToString(xright, 0) + "  " + ofToString(ofGetWidth() - xright - _ww, 0) + "\n";
		s3 += "yBottom    " + ofToString(ybottom, 0) + "  " + ofToString(ofGetHeight() - ybottom - _hh, 0) + "\n\n";
		s3 += "xCenter    " + ofToString(xcenter, 0) + "\n";
		s3 += "yCenter    " + ofToString(ycenter, 0) + "\n\n";
		s3 += "TopR       " + ofToString(r.getTopRight().x, 0) + ", " + ofToString(r.getTopRight().y, 0) + "\n";
		s3 += "BottomR    " + ofToString(r.getBottomRight().x, 0) + ", " + ofToString(r.getBottomRight().y, 0) + "\n\n";
		s3 += "Window     " + ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight()) /*+ "\n\n"*/;
		//s3 += "Drag Diff  " + ofToString(rect_Box.diffx) + ", " + ofToString(rect_Box.diffy);

		p = p + glm::vec2(0, 40);
		ofDrawBitmapStringHighlight(s3, p, 0, 255);
	}

	//--------------------------------------------------------------
	void doForceFitOnWindow()
	{
		// Size
		if (rect_Box.getWidth() > ofGetWidth() - 2 * xpad)
		{
			rect_Box.setWidth(ofGetWidth() - 2 * xpad);
		}
		else if (rect_Box.getHeight() > ofGetHeight() - 2 * ypad)
		{
			rect_Box.setHeight(ofGetHeight() - 2 * ypad);
		}

		//--

		// Position
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
	}

	//--

private:

	//--------------------------------------------------------------
	void drawDoubleClicker()
	{
		if (!bGui) return;

		//--

		// 1. Double click to swap edit mode

		//--

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
				// set free layout mode
				if (bStateEdit)
				{
					if (modeLayout != FREE_LAYOUT) modeLayout = FREE_LAYOUT;
				}

				return;
			}
		}

		//--

		// 2. Left pressed + right click : to close box!

		//if (this->isEditing())
		{
			//if (doubleClicker.isMouseRightPressedThenPressedLeft()) 
			if (ofGetMousePressed(0) && doubleClicker.isMouseRightClick())
			{
				ofLogWarning("ofxSurfingBoxInteractive") << " " << (__FUNCTION__);

				bGui = false;

				return;
			}
		}

		//--

		// 3. Right click to swap modeLayout mode

		if (doubleClicker.isMouseRightClick())
		{
			// 3. Triple clicks swap modeLayout mode
			//if (doubleClicker.isMouseTripleClick())
			{
				bState2 = !bState2;

				int i = modeLayout;
				i++;

				if (i >= NUM_LAYOUTS) { modeLayout = FREE_LAYOUT; }
				else { modeLayout = BOX_LAYOUT(i); }

				bIsChanged = true;

				return;
			}
		}

		//--

		if (bDebugDoubleClick) doubleClicker.draw();
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

			//doubleClicker.enableAllEvents();
		}
		else
		{
			rect_Box.disableEdit();

			//TODO: autosave
			// Save
			//rect_Box.saveSettings(path_RectHelpBox, path_Global + "/", false);

			//doubleClicker.disableAllEvents();
		}

		bIsChanged = true;
	}

	//--------------------------------------------------------------
	void setToggleMode(bool bBack = false) {
		int i = BOX_LAYOUT(modeLayout);

		if (!bBack) i++; // next
		else // previous
		{
			if (i == FREE_LAYOUT) i = BOTTOM_RIGHT;
			else i--;
		}

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

	//--------------------------------------------------------------
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
