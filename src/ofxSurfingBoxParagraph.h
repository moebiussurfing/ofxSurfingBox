
#pragma once

/*

	TODO:

	+ add shadow mode
	+ add align

*/

//----

#include "ofMain.h"

#include "ofxSurfingBoxInteractive.h"

#include "ofxFontStash2.h"

class ofxSurfingBoxParagraph : public ofxSurfingBoxInteractive
{

public:

	ofxFontStash2::Fonts fonts;

private:

	string text_Styled = "";
	string text_Body = "";
	string text_Title = "";
	string text_TitleRaw = "";
	string s;

	bool bStyled = false;
	bool bUseTitle = false;

	float round = 5;

	ofColor _colorText; // lines and text color
	ofColor _colorBg; // background color
	ofColor _colorShadow; // bg selected button
	bool _bUseShadow;

	//--

	// styles
	float size_Title1;
	float size_Title2;
	float size_Style1;
	float size_Style2;
	float size_Style3;

	string font_Style1;
	string font_Style2;

	//--

	float xpad = 20;
	float ypad = 20;

	ofRectangle bbox;
	ofRectangle bboxHeader;

	ofParameter<bool> bThemeDarkOrLight{ "Theme", true };

public:

	//--------------------------------------------------------------
	virtual void setup()
	{
		setup(true);
	}

	//--------------------------------------------------------------
	void setup(bool bWithText)
	{
		ofLogNotice("ofxSurfingBoxHelpText") << (__FUNCTION__);

		// colors
		params_AppSession.add(bThemeDarkOrLight);

		this->ofxSurfingBoxInteractive::setup();

		_bUseShadow = false;

		//--

		fonts.setup(false);

		size_Title1 = 45;
		size_Title2 = 35;
		size_Style1 = 25;
		size_Style2 = 15;
		size_Style3 = 10;

		font_Style1 = FONT_FILES_PATH + ofToString(FONT_FILE_BIG);
		font_Style2 = FONT_FILES_PATH + ofToString(FONT_FILE_SMALL);

		fonts.addFont("F1", font_Style1);
		fonts.addFont("F2", font_Style2);

		buildFonts(_colorText);

		fonts.pixelDensity = 2.0;

		// height is forced
		this->setLockH(true);

		// constraint
		//this->setRectConstraintMin(glm::vec2(350, 50));

		// Startup
		setTheme(bThemeDarkOrLight);
	}

private:

	//--------------------------------------------------------------
	void buildFonts(ofColor __colorText)
	{
		// clear styles
		auto styles = fonts.getStyles();
		for (auto& it : styles) {
			fonts.removeStyle(it.first);
		}

		// define font styles
		fonts.addStyle("header", ofxFontStash2::Style("F1", size_Title1, __colorText));
		fonts.addStyle("header2", ofxFontStash2::Style("F1", size_Title2, __colorText));

		fonts.addStyle("body", ofxFontStash2::Style("F2", size_Style1, __colorText));
		fonts.addStyle("bold", ofxFontStash2::Style("F2", size_Style2, __colorText));//TODO: add ttf
		fonts.addStyle("italic", ofxFontStash2::Style("F2", size_Style2, __colorText));//TODO: add ttf

		fonts.addStyle("H1", ofxFontStash2::Style("F2", size_Title2, __colorText));
		fonts.addStyle("H2", ofxFontStash2::Style("F2", size_Style1, __colorText));
		fonts.addStyle("H3", ofxFontStash2::Style("F2", size_Style2, __colorText));
		fonts.addStyle("H4", ofxFontStash2::Style("F2", size_Style3, __colorText));
	}

public:

	//--------------------------------------------------------------
	void setTitle(string text) {
		// we add plain text without tags easily

		bUseTitle = true;
		text_TitleRaw = text;

		text_Title = "<header>";
		text_Title += text;
		text_Title += "</header>";
	}

	//--------------------------------------------------------------
	void setText(string text) {
		// we add plain text without tags easily

		bStyled = false;

		text_Body = "<body>";
		text_Body += text;
		text_Body += "</body>";
	}

	//--------------------------------------------------------------
	void setTextStyled(string text) {
		// text content includes custom style tags!

		bStyled = true;

		text_Styled = text;
	}

	//--------------------------------------------------------------
	virtual void draw()
	{
		if (!bGui) return;

		this->ofxSurfingBoxInteractive::draw(true);

		float x = this->getX() + xpad;
		float y = this->getY() + size_Title1 + ypad;
		float w = this->getWidth() - 2 * xpad;

		//--

		// Bg

		ofColor colorBg;

		if (this->ofxSurfingBoxInteractive::modeLayout == BOX_LAYOUT::FREE_LAYOUT)
		{
			if (this->isEditing())
			{
				float a = ofxSurfingHelpers::getFadeBlink(0.6f, 1.f);
				ofColor c = ofColor(_colorBg, _colorBg.a * a);
				this->ofxSurfingBoxInteractive::rBox.draw();
				colorBg = c;
			}
			else colorBg = _colorBg;
		}
		else colorBg = _colorBg;

		ofPushStyle();
		ofSetColor(colorBg);
		ofFill();
		ofDrawRectRounded(this->getRectangle(), round);
		ofPopStyle();

		//--

		// Text

		s = "";

		if (bStyled)
		{
			s = text_Styled;
		}
		else
		{
			if (bUseTitle)
			{
				s += text_Title;
			}

			// body
			s += text_Body;
		}

		//TODO: requires dual styles..
		// shadow
		if (_bUseShadow)
		{
			buildFonts(_colorShadow);//TODO: could be slow?
			fonts.drawFormattedColumn(s, x + 1, y + 1, w, OF_ALIGN_HORZ_LEFT, this->bDebug);
		}

		// draw
		if (_bUseShadow) buildFonts(_colorText);//TODO: could be slow?
		bbox = fonts.drawFormattedColumn(s, x, y, w, OF_ALIGN_HORZ_LEFT, this->bDebug);

		// force height
		this->setHeight(bbox.getHeight() + size_Title1 + 2 * ypad);

		//--

		//TODO:
		// constraint
		bool bConstraint = 1;

		if (bConstraint)
		{
			// title
			ofxFontStash2::Style style = ofxFontStash2::Style("F1", size_Title1, 255);
			bboxHeader = fonts.getTextBounds(text_TitleRaw, style, 0, 0);

			// force width to fit title as minimum width
			float w1 = bbox.getWidth() + 2 * xpad;
			float w2 = bboxHeader.getWidth() + 2 * xpad;

			float w0;
			if (w1 < w2) w0 = w2;
			else w0 = w1;
			this->setWidth(w0);

			//cout << "w1:" << w1 << endl;
			//cout << "w2:" << w2 << endl;
			//cout << "w0:" << w0 << endl<< endl;
		}
		else
		{
			float w1 = bbox.getWidth() + 2 * xpad;
			this->setWidth(w1);
		}
	}

	//--------------------------------------------------------------
	void setRounded(int r = 5) {
		round = r;
	}

	////--------------------------------------------------------------
	//void setPadBox(int pad = 50) { // Inner padding between box borders an text paragraph
	//	tpad = pad;
	//}
	//--------------------------------------------------------------
	void setPads(float pad = 4) { // Pad from box to window borders
		xpad = ypad = pad;
	}
	//--------------------------------------------------------------
	void setPadX(float pad = 4) { // Pad from box to window borders
		xpad = pad;
	}
	//--------------------------------------------------------------
	void setPadY(float pad = 4) { // Pad from box to window borders
		ypad = pad;
	}

	//--

	////--------------------------------------------------------------
	//void setFontSize(int size = 10) { // Call before setup. Default it's 10
	//	size_Style1 = size;
	//}
	////--------------------------------------------------------------
	//void setFontName(string name) { // Set the name only. path is /assets/fonts/*name*
	//	font_Style1 = name;
	//}

	////--------------------------------------------------------------
	//void setFontTitleSize(int size = 10) { // Call before setup. Default it's 10
	//	size_Style2 = size;
	//}
	////--------------------------------------------------------------
	//void setFontTitleName(string name) { // Set the name only. path is /assets/fonts/*name*
	//	font_Style2 = name;
	//}

	//--

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

		buildFonts(_colorText);
	}

	//--

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
};