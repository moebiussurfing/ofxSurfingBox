
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

	+ add shift + mouse wheel to scale from center.

	+ add ofRectangle helpers to maximize, full screen.
		or a new types like top left, full width bottom, top center...etc.
		fit screen or mini / big modes to use on a video player.
	+ check colliding when multi instances.?
	+ fix that setLockX is overwritten with loading settings..?
	+ store layout mode, not only the free layout.?
	+ update text box from this class.?

*/


// Helper to get name
#define AUTO_CASE_CREATE(a) case a: return #a

//--

namespace ofxSurfingBox
{
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

		//TODO: 
		FULL_SCREEN,
		FULL_WITH_TOP,
		FULL_WITH_BOTTOM,

		NUM_LAYOUTS
	};

	enum BOX_TYPE
	{
		TYPE_RECTANGLE = 0,
		TYPE_BAR_VERTICAL,
		TYPE_BAR_HORIZONTAL,

		NUM_TYPES
	};
}

using namespace ofxSurfingBox;

namespace ofxSurfingBox {

	class ofxSurfingBoxInteractive
	{
	public:

		void setType(BOX_TYPE mode) {
			modeType = mode;
			iModeType = (int)modeType;

			switch (modeType)
			{
			case TYPE_RECTANGLE: {
				str_modeType = getTypeName();
				//workflow.
				//TODO: should not expect to block previous state if settled manually by the coder/user..
				setLockH(false);
				setLockW(false);
				break;
			}
			case TYPE_BAR_VERTICAL: {
				str_modeType = getTypeName();
				setLockH(true);
				setLockW(false);
				modeLayout = FREE_LAYOUT;
				iModeLayout = (int)modeLayout;
				setY(0);
				setHeight(ofGetHeight());
				break;
			}
			case TYPE_BAR_HORIZONTAL: {
				str_modeType = getTypeName();
				setLockH(false);
				setLockW(true);
				modeLayout = FREE_LAYOUT;
				iModeLayout = (int)modeLayout;
				setX(0);
				setWidth(ofGetWidth());
				break;
			}
			}

			if (bForceType) doForceType();
		};

		//--------------------------------------------------------------
		void setToggleType(bool bBack = false) {
			int i = BOX_TYPE(modeType);

			if (!bBack) i++; // next
			else // previous
			{
				if (i == TYPE_RECTANGLE) i = NUM_TYPES - 1;//cycle
				else i--;
			}

			i = i % NUM_TYPES;
			modeType = BOX_TYPE(i);
			setType(modeType);
		};

		//--------------------------------------------------------------
		string getTypeName() {
			string s = "";

			switch (modeType)
			{
			case TYPE_RECTANGLE: {
				s = "RECTANGLE";
				break;
			}
			case TYPE_BAR_VERTICAL: {
				s = "VERTICAL";
				break;
			}
			case TYPE_BAR_HORIZONTAL: {
				s = "HORIZONTAL";
				break;
			}
			}

			return s;
		}

		//--------------------------------------------------------------
		string getModeName() {

			switch (modeLayout)
			{
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

				//TODO: 
				AUTO_CASE_CREATE(FULL_SCREEN);
				AUTO_CASE_CREATE(FULL_WITH_TOP);
				AUTO_CASE_CREATE(FULL_WITH_BOTTOM);

			default: return string("UNKNOWN LAYOUT");
			}
		};

		//--

	public:
		//--------------------------------------------------------------
		ofxSurfingBoxInteractive()
		{
			ofAddListener(ofEvents().exit, this, &ofxSurfingBoxInteractive::exit, OF_EVENT_ORDER_AFTER_APP);

			setRectConstraintMin(glm::vec2(50, 50));
			setPads(4, 4);
		};

		//--------------------------------------------------------------
		~ofxSurfingBoxInteractive()
		{
			ofRemoveListener(ofEvents().exit, this, &ofxSurfingBoxInteractive::exit);
		};

	private:

		//--------------------------------------------------------------
		virtual void exit(ofEventArgs& args)
		{
			exit();
		}

		//--------------------------------------------------------------
		virtual void exit()
		{
			bEdit.removeListener(this, &ofxSurfingBoxInteractive::Changed_Edit);
			bUseBorder.removeListener(this, &ofxSurfingBoxInteractive::Changed_Border);
			bGui.removeListener(this, &ofxSurfingBoxInteractive::Changed_bGui);

			// Settings

			ofxSurfingHelpers::CheckFolder(path_Global + "/");

			// Restore the one from free layout mode before save
			if (modeLayout != FREE_LAYOUT) rBox.set(rect_Box_STORE);

			rBox.saveSettings(path_RectHelpBox, path_Global + "/", false);

			iModeLayout = (int)modeLayout;//read value before save
			iModeType = (int)modeType;//read value before save

			ofxSurfingHelpers::saveGroup(params_AppSession, path_Global + "/" + path_AppSession);
		};

		//--

	public:

		//--------------------------------------------------------------
		void setup()
		{
			setup(true);
		};

		//--------------------------------------------------------------
		void setup(bool b)
		{
			bEdit.set("Edit", false);
			bUseBorder.set("Border", false);
			bUseBorderBlinking.set("Blink", false);
			bTransparent.set("Transparent", false);
			iModeLayout.set("ModeLayout", 0, 0, NUM_LAYOUTS - 1);
			iModeType.set("ModeType", 0, 0, NUM_TYPES - 1);

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
			rBox.loadSettings(path_RectHelpBox, path_Global + "/");

			// Store
			rect_Box_STORE = rBox.getRect();

			rBox.setEnableMouseWheel(true);
			rBox.setAutoSave(false);

			//--

			// Extra settings
			params_AppSession.add(bEdit);
			params_AppSession.add(iModeLayout);
			params_AppSession.add(iModeType);
			params_AppSession.add(bTransparent);
			params_AppSession.add(bUseBorder);
			params_AppSession.add(bUseBorderBlinking);

			ofxSurfingHelpers::loadGroup(params_AppSession, path_Global + "/" + path_AppSession);

			modeLayout = BOX_LAYOUT(iModeLayout.get());
			modeType = BOX_TYPE(iModeType.get());

			setType(modeType);
		};

		//--------------------------------------------------------------
		void drawHelpInfo()
		{
			auto addLineDivider = [](string& _s) {
				_s += "\n ---------------------------------- ";
			};

			string s;

			s += "\n ofxSurfingBoxInteractive";
			addLineDivider(s);
			s += "\n";
			s += "\n";
			s += "\n";
			s += "\n";
			s += "\n * INTERNAL";
			s += "\n";
			s += "\n";
			s += "\n DOUBLE LEFT-CLICK : EDIT " + ofToString(this->isEditing() ? "ON" : "OFF");
			s += "\n";
			addLineDivider(s);
			s += "\n";
			s += "\n MODE LAYOUT: \n";
			s += "\n #" + ofToString(this->getModeLayout()) + " " + this->getModeName() + "\t";
			s += "\n";
			s += "\n [LEFT] : PREV";
			s += "\n [RIGHT][TAB] : NEXT";
			s += "\n RIGHT-CLICK  : NEXT";
			s += "\n";
			s += "\n MODE TYPE: \n";
			s += " [T] : TYPE " + ofToString(this->getModeTypeName());
			s += "\n";
			addLineDivider(s);
			s += "\n";
			s += "\n MOUSE WHEEL TO RESIZE:";
			s += "\n\  FROM TOP LEFT";
			s += "\n\  + CTRL FROM CENTER";
			s += "\n\  + SHIFT WIDTH ONLY";
			s += "\n\  + ALT HEIGHT ONLY";
			s += "\n";
			addLineDivider(s);
			s += "\n";
			s += "\n LEFT-CLICK + RIGHT-CLICK : HIDE";
			s += "\n";
			addLineDivider(s);
			s += "\n";
			s += "\n";
			s += "\n";
			s += "\n";
			s += "\n";
			s += "\n * EXTERNAL (ofApp)";
			s += "\n";
			s += "\n";
			s += "\n [G]         VISIBLE " + ofToString(this->isVisible() ? "ON" : "OFF");
			s += "\n [SPACE]     EDIT " + ofToString(this->isEditing() ? "ON" : "OFF");
			s += "\n [BACKSPACE] RESET";
			s += "\n";
			s += "\n [D]         DEBUG " + ofToString(this->isDebug() ? "ON" : "OFF");
			s += "\n [B]         BORDER " + ofToString(this->isBorder() ? "ON" : "OFF");
			s += "\n [K]         BORDER BLINK " + ofToString(this->isBorderBlinking() ? "ON" : "OFF");
			s += "\n [A]         LOCK ASPECT RATIO " + ofToString(this->isLockedAspectRatio() ? "ON" : "OFF");
			s += "\n";

			ofDrawBitmapStringHighlight(s, 15, 28, ofColor(0, 100), ofColor(255));
		};

		//--------------------------------------------------------------
		void draw()
		{
			draw(true);
		};

		//--------------------------------------------------------------
		void draw(bool b)//if passed false will skip drawing.
		{
			if (!b) return;

			if (!bGui) return;

			ofPushStyle();
			{
				static float _h_PreFullScreen = 0;

				//--

				// Simple callbacks

				static BOX_LAYOUT modeLayout_PRE = NUM_LAYOUTS;
				static bool bLockedAspectRatio_PRE = !bLockedAspectRatio;

				//--

				//TODO:
				// Check if rectangle changed
				// Pos / Size 
				if (rBox.getRect() != rBox_PRE)
				{
					rBox_PRE = rBox.getRect();
					bIsChanged = true;
				}

				//--

				// Changed Mode

				// Memorize free layout mode 
				// to not be overwritten by 
				// other predefined (top,left..) layout positions.

				if (modeLayout != modeLayout_PRE) // changed
				{
					str_modeLayout = getModeName();

					//--

					// Extras

					if (modeLayout_PRE == FULL_SCREEN ||
						modeLayout_PRE == FULL_WITH_TOP ||
						modeLayout_PRE == FULL_WITH_BOTTOM)
					{
						rBox.set(rect_Box_STORE);
					}

					//--

					//TODO:
					//if (modeLayout == FULL_SCREEN ||
					//	modeLayout == FULL_WITH_TOP ||
					//	modeLayout == FULL_WITH_BOTTOM)
					//{
					//	rect_Box_STORE = rBox.getRect();
					//}

					//--

					// If we are leaving free mode, 
					// we must store that rectangle.
					if (modeLayout_PRE == FREE_LAYOUT)
					{
						// Store
						rect_Box_STORE = rBox.getRect();
					}
					// Changed to free layout
					else if (modeLayout == FREE_LAYOUT)
					{
						// Restore
						rBox.set(rect_Box_STORE);
					}

					//----

					// Update flag
					modeLayout_PRE = modeLayout;
					bIsChanged = true;

					//--

					// workflow
					if (modeLayout != FREE_LAYOUT)
					{
						setEdit(false);
					}
				}

				//--

				// Calculate anchor points to define layout

				// window
				float _w = ofGetWidth();
				float _h = ofGetHeight();

				// box size
				float _ww = rBox.getWidth();
				float _hh = rBox.getHeight();

				// box pos
				float _xx = 0;
				float _yy = 0;

				//--

				if (modeLayout == FREE_LAYOUT)
				{
					_xx = rBox.getX();
					_yy = rBox.getY();
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

					// Extra

					//TODO:
					else if (modeLayout == FULL_SCREEN)
					{
						_xx = xleft;
						_yy = ytop;
						_h_PreFullScreen = rBox.getHeight();//store
						float __w = ofGetWidth() - 2 * xleft;
						float __h = ofGetHeight() - 2 * ytop;
						if (_ww != __w) rBox.setWidth(__w);
						if (_hh != __h) rBox.setHeight(__h);

						////TODO:
						//_ww = rBox.getWidth();
						//_hh = rBox.getHeight();
					}

					else if (modeLayout == FULL_WITH_TOP)
					{
						_xx = xleft;
						_yy = ytop - _hh;

						float __w = ofGetWidth() - 2 * xleft;
						if (_ww != __w) rBox.setWidth(__w);

						//TODO:
						_ww = rBox.getWidth();
						_hh = rBox.getHeight();
					}

					else if (modeLayout == FULL_WITH_BOTTOM)
					{
						_xx = xleft;
						_yy = ofGetHeight() - ytop;

						float __w = ofGetWidth() - 2 * xleft;
						if (_ww != __w) rBox.setWidth(__w);

						//TODO:
						_ww = rBox.getWidth();
						_hh = rBox.getHeight();
					}

					//--

					// Set

					rBox.setX(_xx);
					rBox.setY(_yy);
				}

				//--

				// Calculate fit/clamp Marks
				{
					//TODO: ?
					xcenter = _w / 2.f - _ww / 2.f;
					ycenter = _h / 2.f - _hh / 2.f;

					if (modeType == TYPE_BAR_HORIZONTAL) {
						xleft = 0;
						xright = _w;
					}
					else
					{
						xleft = xpad;
						xright = _w - _ww - xpad;
					}

					if (modeType == TYPE_BAR_VERTICAL) {
						ytop = 0;
						ybottom = _h;
					}
					else {
						ytop = ypad;
						ybottom = _h - _hh - ypad;
					}
				}

				//--

				// Force type
				if (bForceType) doForceType();

				// Force fit box inside the window
				if (bForceFitInsideWindow) doForceFitOnWindow();

				//--

				// Move clicker handler linked to the box
				doubleClicker.set(_xx, _yy, _ww, _hh);

				// Double / right clicker handler
				drawDoubleClicker();

				//--

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
					if (rBox.isEditing())
					{
						rBox.draw();
					}
				}

				//--

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

				//--

				// Debug
				// Detect changes
				// types and change colors.
				if (bDebug) drawDebug();
			}
			ofPopStyle();
		}

		//--

	private:

		ofxInteractiveRect rBox = { "_" };

	public:

		ofxInteractiveRect& getInteractiveRect() { return rBox; };

		//--

		// Exposed visible toggle to be used
		// or linked in other parent scope GUIs!
		ofParameter<bool> bGui{ "Box", true };

		//--

	private:

		BOX_LAYOUT modeLayout = FREE_LAYOUT;
		string str_modeLayout = "";

		BOX_TYPE modeType = TYPE_RECTANGLE;
		string str_modeType = "";

		ofRectangle rBox_PRE;
		ofRectangle rect_Box_STORE;

		//--

	public:

		ofParameter<bool> bEdit;

	private:
		ofParameter<bool> bTransparent;
		ofParameter<bool> bUseBorder;
		ofParameter<bool> bUseBorderBlinking;
		ofParameter<int> iModeLayout; // only used to load/store settings
		ofParameter<int> iModeType; // only used to load/store settings


	private:
		std::string path_RectHelpBox = "myBox"; // default name
		std::string path_Global = "ofxSurfingBoxInteractive/"; // default path
		std::string suffixSettings = "Settings.json";
		std::string path_AppSession = path_RectHelpBox + "_" + suffixSettings;

		ofColor _colorButton; // bg selected button
		ofColor _colorBg; // background color
		ofColor _colorShadow;
		ofColor _colorBorder;

		bool bIsEditing = false;

		bool bLocked = false;
		bool bLockedAspectRatio = false;

		bool bForceFitInsideWindow = true;
		bool bForceType = true;

	public:

		ofParameterGroup params_AppSession{ "Settings" };

	private:
		bool bIsChanged = false;
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

		DoubleClicker doubleClicker;//handles double clicks and right click interaction

		bool bStateEdit = false;
		//bool bState2 = false;

		bool bEnabledFileSettings = true;

		//--

	public:

		// Lock drag borders
		//--------------------------------------------------------------
		void setLockX(bool b = true) { rBox.setLockX(b); };//set enable/disable drag x interaction
		//--------------------------------------------------------------
		void setLockY(bool b = true) { rBox.setLockY(b); };//set enable/disable drag y interaction
		//--------------------------------------------------------------
		void setLockW(bool b = true) { rBox.setLockW(b); };//set enable/disable drag width interaction
		//--------------------------------------------------------------
		void setLockH(bool b = true) { rBox.setLockH(b); };//set enable/disable drag height interaction

		//--

		// Debug DoubleClick

	private:

		bool bDebugDoubleClick = false;

	public:

		//--------------------------------------------------------------
		void setDebugDoubleClick(bool b) {
			bDebugDoubleClick = b;
			doubleClicker.setDebug(bDebugDoubleClick);
		};
		//--------------------------------------------------------------
		void setToggleDebugDoubleClick() {
			bDebugDoubleClick = !bDebugDoubleClick;
			doubleClicker.setDebug(bDebugDoubleClick);
			bIsChanged = true;
		};
		//--------------------------------------------------------------
		bool isDebugDoubleClick() const {
			return bDebugDoubleClick;
		};
		//--------------------------------------------------------------
		bool isBorder() const {
			return bUseBorder;
		};
		//--------------------------------------------------------------
		bool isBorderBlinking() const {
			return bUseBorderBlinking;
		};

		//--------------------------------------------------------------
		bool isDebug() const {
			return bDebug;
		};

		// A simple callback to trig when theme or layout changed
		//--------------------------------------------------------------
		bool isChanged() {
			if (!bIsChanged) return false;
			else
			{
				bIsChanged = false;
				return true;
			}
		};

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
		};

		//--------------------------------------------------------------
		bool isEditing() {

			bool b = bIsEditing;

			return b;
		};

		//--------------------------------------------------------------
		BOX_LAYOUT getModeLayout() {
			return modeLayout;
		};
		//--------------------------------------------------------------
		string getModeLayoutName() {
			return getModeName();
		};

		//--------------------------------------------------------------
		BOX_TYPE getModeType() {
			return modeType;
		};
		//--------------------------------------------------------------
		string getModeTypeName() {
			return getTypeName();
		};

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
		};

		//--

		//--------------------------------------------------------------
		bool isVisible() const { return bGui.get(); };

		//--------------------------------------------------------------
		ofRectangle getRectangle() {
			return rBox;
		};

		//--------------------------------------------------------------
		float getX() { return rBox.x; };
		//--------------------------------------------------------------
		float getY() { return rBox.y; };
		//--------------------------------------------------------------
		float getWidth() { return rBox.getWidth(); };
		//--------------------------------------------------------------
		float getHeight() { return rBox.getHeight(); };

		//--------------------------------------------------------------
		void setUseBorder(bool b) { bUseBorder = b; };

		//--------------------------------------------------------------
		void setTransparent(bool b) { bTransparent = b; };

		//--------------------------------------------------------------
		void setPads(float x, float y) {//call after setup
			xpad = x;
			ypad = y;
			rBox.setPads(xpad, ypad);
		};

		//--------------------------------------------------------------
		void setUseBorderBlinking(bool b) {
			bUseBorderBlinking = b;
			if (b) bUseBorder = true;
		};
		//--------------------------------------------------------------
		void setToggleUseBorder() {
			bUseBorder = !bUseBorder;
		};
		//--------------------------------------------------------------
		void setToggleUseBorderBlinking() {
			bUseBorderBlinking = !bUseBorderBlinking;
			if (bUseBorderBlinking) bUseBorder = true;
		};

		//--------------------------------------------------------------
		void setBorderColor(ofColor c) { _colorBorder = c; };

		//--------------------------------------------------------------
		//void setWokflow(bool b) { bWorflow = b; } // enables some automated workflow. ex: disable gui edit when hide.

		//--------------------------------------------------------------
		void setName(string name, bool bRenamebGui = false) {
			path_RectHelpBox = name;
			if (bRenamebGui) bGui.setName(name);
			path_AppSession = name + "_" + suffixSettings;
		};

		//--

	public:

		//--------------------------------------------------------------
		void setRectConstraintMin(glm::vec2 shape) {
			rBox.setRectConstraintMin(shape);
		};
		//--------------------------------------------------------------
		void setRectConstraintMax(glm::vec2 shape) {
			rBox.setRectConstraintMax(shape);
		};

		//--

		//--------------------------------------------------------------
		void setRectangle(ofRectangle shape) {
			this->setShape(shape);
		};
		//--------------------------------------------------------------
		void setShape(ofRectangle shape) {
			rBox.setRect(shape.x, shape.y, shape.getWidth(), shape.getHeight());
		};
		//--------------------------------------------------------------
		void setPosition(float x, float y) {
			rBox.setX(x);
			rBox.setY(y);
		};
		//--------------------------------------------------------------
		void setX(float x) {
			rBox.setX(x);
		};
		//--------------------------------------------------------------
		void setY(float y) {
			rBox.setY(y);
		};
		//--------------------------------------------------------------
		void setWidth(float w) {
			rBox.setWidth(w);
		};
		//--------------------------------------------------------------
		void setHeight(float h) {
			rBox.setHeight(h);
		};

		//--

	public:

		//--------------------------------------------------------------
		void setPathGlobal(string path) {//call before setup. Will set path to save settings into.
			path_Global = path;
		};
		// Legacy
		//--------------------------------------------------------------
		void setPath(string path) {//call before setup. Will set path to save settings into.
			path_Global = path;

			ofxSurfingHelpers::CheckFolder(path_Global);
		};

		//--------------------------------------------------------------
		void reset(bool bSave = true, float ratio = 0.8)
		{
			ofRectangle r;
			float ww = ofGetWidth();
			float wh = ofGetHeight();
			r.setFromCenter(ww / 2.f, wh / 2.f, ww * ratio, wh * ratio);

			rBox.set(r);

			modeLayout = FREE_LAYOUT;

			if (bSave) {
				rBox.saveSettings(path_RectHelpBox, path_Global + "/", false);
				ofxSurfingHelpers::saveGroup(params_AppSession, path_Global + "/" + path_AppSession);
			}
		};

		//--------------------------------------------------------------
		void reset(bool bOnlySize /*= false*/, int width /*= 400*/, bool bSave = true) {

			// size
			float sz = width;
			rBox.setWidth(sz);
			rBox.setHeight(sz);

			// pos
			if (!bOnlySize) rBox.setPosition(ofGetWidth() / 2.f - (sz / 2.f), ofGetHeight() / 2.f - (sz / 2.f));

			modeLayout = FREE_LAYOUT;

			if (bSave) {
				rBox.saveSettings(path_RectHelpBox, path_Global + "/");
				ofxSurfingHelpers::saveGroup(params_AppSession, path_Global + "/" + path_AppSession);
			}
		};

	private:
		//--------------------------------------------------------------
		void Changed_Edit(bool& edit) {
			ofLogNotice("ofxSurfingBoxInteractive") << " " << (__FUNCTION__) << "Edit : " << edit;
			setEdit(edit);
		};
		//--------------------------------------------------------------
		void Changed_Border(bool& b) {
			ofLogNotice("ofxSurfingBoxInteractive") << " " << (__FUNCTION__) << "Border: " << b;
			setUseBorder(b);
		};

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
					rBox.disableEdit();
					doubleClicker.disableAllEvents();
				}
			}
		};

		//--

	//public:
	private:
		//--------------------------------------------------------------
		void drawBorder()
		{
			ofPushStyle();
			ofNoFill();
			ofSetColor(_colorBorder);
			ofSetLineWidth(2.0);
			ofDrawRectangle(getRectangle());
			ofPopStyle();
		};

		//--------------------------------------------------------------
		void drawBorderBlinking()
		{
			if (!bGui) return;//TODO:

			int a = ofMap(ofxSurfingHelpers::Bounce(), 0, 1, 12, 48);
			//int a = ofMap(ofxSurfingHelpers::Bounce(), 0, 1, 24, 64);
			ofColor c = ofColor(_colorBorder, a);

			// Border
			ofPushStyle();
			ofNoFill();
			ofSetColor(c);
			ofSetLineWidth(2.0);
			ofDrawRectangle(getRectangle());
			ofPopStyle();
		};

		//--------------------------------------------------------------
		void drawDebug()
		{
			//auto r = this->getRectangle();
			auto r = rBox;

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
			ofDrawBitmapStringHighlight(s1, p, ofColor(c1, 150), 255);

			p = p + glm::vec2(0, 20);
			ofDrawBitmapStringHighlight(s2, p, ofColor(c2, 150), 255);

			string s3 = "";
			s3 += "MODE       " + this->getModeName() + "\n";
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
			//s3 += "Drag Diff  " + ofToString(rBox.diffx) + ", " + ofToString(rBox.diffy);

			p = p + glm::vec2(0, 40);
			ofDrawBitmapStringHighlight(s3, p, ofColor(0, 100), ofColor(255));

		};

	private:
		//--------------------------------------------------------------
		void doForceType()
		{
			switch (modeType)
			{
			case TYPE_RECTANGLE: {
				break;
			}
			case TYPE_BAR_VERTICAL: {
				setY(0);
				setHeight(ofGetHeight());
				break;
			}
			case TYPE_BAR_HORIZONTAL: {
				setX(0);
				setWidth(ofGetWidth());
				break;
			}
			}
		}

		//--------------------------------------------------------------
		void doForceFitOnWindow()
		{
			// Size clamp

			float xgap = 0;
			if (modeType != TYPE_BAR_HORIZONTAL) xgap = 2 * xpad;
			if (rBox.getWidth() > ofGetWidth() - xgap)
			{
				rBox.setWidth(ofGetWidth() - xgap);
			}

			float ygap = 0;
			if (modeType != TYPE_BAR_VERTICAL) ygap = 2 * ypad;
			if (rBox.getHeight() > ofGetHeight() - ygap)
			{
				rBox.setHeight(ofGetHeight() - ygap);
			}

			//--

			// Position clamp
			if (rBox.getY() > ybottom) // bottom
			{
				rBox.setY(ybottom);
			}
			else if (rBox.getY() < ytop) // top
			{
				rBox.setY(ytop);
			}
			else if (rBox.getX() < xleft) // left
			{
				rBox.setX(xleft);
			}
			else if (rBox.getX() > xright) // right
			{
				rBox.setX(xright);
			}
		};

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
					//bState2 = !bState2;

					//int i = modeLayout;
					//i++;
					//if (i >= NUM_LAYOUTS) { modeLayout = FREE_LAYOUT; }
					//else { modeLayout = BOX_LAYOUT(i); }
					//bIsChanged = true;
					//return;

					this->setToggleModeLayout();
				}
			}

			//--

			if (bDebugDoubleClick) doubleClicker.draw();
		};

	public:

		//--------------------------------------------------------------
		void setToggleEdit()
		{
			setEdit(!rBox.isEditing());
		};

		//--------------------------------------------------------------
		void setEdit(bool edit)
		{
			bIsEditing = edit;

			if (bEdit != bIsEditing) bEdit = bIsEditing;

			if (bEdit)
			{
				rBox.enableEdit();

				// workflow
				if (modeLayout != FREE_LAYOUT)
				{
					modeLayout = FREE_LAYOUT;
				}

				//doubleClicker.enableAllEvents();
			}
			else
			{
				rBox.disableEdit();

				//TODO: auto save
				// Save
				//rBox.saveSettings(path_RectHelpBox, path_Global + "/", false);

				//doubleClicker.disableAllEvents();
			}

			bIsChanged = true;
		};

		//--------------------------------------------------------------
		void setToggleModeLayout(bool bBack = false) {
			int i = BOX_LAYOUT(modeLayout);

			if (!bBack) i++; // next
			else // previous
			{
				if (i == FREE_LAYOUT) i = NUM_LAYOUTS - 1;//cycle
				else i--;
			}

			i = i % NUM_LAYOUTS;
			modeLayout = BOX_LAYOUT(i);
		};
		//--------------------------------------------------------------
		void setModeLayout(BOX_LAYOUT mode) {
			modeLayout = mode;
		};
		//--------------------------------------------------------------
		void setLocked(bool b) {//enable/disable move and resize and all interaction
			bLocked = b;
			if (b) doubleClicker.disableAllEvents();
			else doubleClicker.enableAllEvents();
		};
		//--------------------------------------------------------------
		void setToggleLocked() {
			bLocked = !bLocked;
			setLocked(bLocked);
		};

	private:

		//--------------------------------------------------------------
		bool isLocked() { return bLocked; };

		//--

	public:

		//--------------------------------------------------------------
		void setVisible(bool b) {
			bGui = b;
		};
		//--------------------------------------------------------------
		void setToggleVisible() {
			bGui = !bGui;
		};

		//--

		//--------------------------------------------------------------
		void setLockAspectRatio(bool b = true) {
			bLockedAspectRatio = b;
			rBox.setLockAspectRatio(bLockedAspectRatio);
		};
		//--------------------------------------------------------------
		void setToggleLockAspectRatio() {
			bLockedAspectRatio = !bLockedAspectRatio;
			rBox.setLockAspectRatio(bLockedAspectRatio);
		};
		//--------------------------------------------------------------
		bool isLockedAspectRatio() {
			return bLockedAspectRatio;
		};
	};

}