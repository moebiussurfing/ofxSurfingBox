
#pragma once

/*

	TODO:

	+ add shadow mode

*/

//----

#include "ofMain.h"

#include "ofxSurfingBoxInteractive.h"

#include "ofxFontStash2.h"

class ofxSurfingBoxParagraph : public ofxSurfingBoxInteractive
{

private:

	ofxFontStash2::Fonts fonts;

	string text_Styled = "";
	string text_Body = "";
	string text_Title = "";
	string text_TitleRaw = "";

	bool bStyled = false;
	bool bUseTitle = false;

	float round = 5;

	// styles
	float size_Title;

	string name_TTF1;
	float size_TTF1;

	string name_TTF2;
	float size_TTF2;

	float xpad = 20;
	float ypad = 20;

	ofRectangle bbox;
	ofRectangle bboxHeader;

	ofColor _colorText; // lines and text color
	ofColor _colorBg; // background color
	ofColor _colorShadow; // bg selected button
	bool _bUseShadow;

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
		_bUseShadow = true;
		params_AppSession.add(bThemeDarkOrLight);

		this->ofxSurfingBoxInteractive::setup();


		//--

		fonts.setup(false);

		size_Title = 44;
		size_TTF1 = 25;
		size_TTF2 = 15;

		name_TTF1 = FONT_FILES_PATH + ofToString(FONT_FILE_BIG);
		name_TTF2 = FONT_FILES_PATH + ofToString(FONT_FILE_SMALL);

		fonts.addFont("F1", name_TTF1);
		fonts.addFont("F2", name_TTF2);

		buildFonts();

		//ofDisableAntiAliasing(); // to get precise lines

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
	void buildFonts()
	{
		//add fonts to the stash
		//fonts.addFont("robo", "fonts/Roboto-Regular.ttf");
		//fonts.addFont("roboBold", "fonts/Roboto-Bold.ttf");
		//fonts.addFont("roboItalic", "fonts/Roboto-Italic.ttf");
		//fonts.addFont("roboBlack", "fonts/Roboto-Black.ttf");
		////define font styles
		//fonts.addStyle("header", ofxFontStash2::Style("roboBlack", size_Title, ofColor::white));
		//fonts.addStyle("body", ofxFontStash2::Style("robo", 18, ofColor(244)));
		//fonts.addStyle("bodyBold", ofxFontStash2::Style("roboBold", 18, ofColor::white));
		//fonts.addStyle("bodyItalic", ofxFontStash2::Style("roboItalic", 18, ofColor::white));
		//fonts.addStyle("bodyRed", ofxFontStash2::Style("robo", 18, ofColor::red));
		//fonts.addStyle("bodyGreen", ofxFontStash2::Style("robo", 18, ofColor::green));
		//fonts.addStyle("bodyBlue", ofxFontStash2::Style("robo", 18, ofColor::blue));
		//fonts.addStyle("bodyDarkgreen", ofxFontStash2::Style("robo", 18, ofColor::darkGreen));

		//--

		// clear styles
		auto styles = fonts.getStyles();
		for (auto& it : styles) {
			fonts.removeStyle(it.first);
		}

		// define font styles
		fonts.addStyle("header", ofxFontStash2::Style("F1", size_Title, _colorText));
		fonts.addStyle("body", ofxFontStash2::Style("F2", size_TTF1, _colorText));
		fonts.addStyle("bodyBold", ofxFontStash2::Style("F2", size_TTF2, _colorText));
		fonts.addStyle("bodyItalic", ofxFontStash2::Style("F2", size_TTF2, _colorText));
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
		float y = this->getY() + size_Title + ypad;
		float w = this->getWidth() - 2 * xpad;

		//----

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

		string s = "";

		if (bStyled)
		{
			s = text_Styled;
		}
		else
		{
			if (bUseTitle)
			{
				s += text_Title;

				//TODO:
				// constraint
				bool bConstraint = false;
				if (bConstraint) {
					// title
					ofxFontStash2::Style style = ofxFontStash2::Style("F1", size_Title, 255);
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
				else {
					float w1 = bbox.getWidth() + 2 * xpad;
					this->setWidth(w1);
				}
			}

			// body
			s += text_Body;
		}

		////TODO: requires dual styles..
		//// shadow
		//if (_bUseShadow)
		//{
		//	ofSetColor(_colorShadow);
		//	fonts.drawFormattedColumn(s, x + 1, y + 1, w, OF_ALIGN_HORZ_LEFT, this->bDebug);
		//}

		// draw
		bbox = fonts.drawFormattedColumn(s, x, y, w, OF_ALIGN_HORZ_LEFT, this->bDebug);

		// force height
		this->setHeight(bbox.getHeight() + size_Title + 2 * ypad);
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

	//--------------------------------------------------------------
	void setFontSize(int size = 10) { // Call before setup. Default it's 10
		size_TTF1 = size;
	}
	//--------------------------------------------------------------
	void setFontName(string name) { // Set the name only. path is /assets/fonts/*name*
		name_TTF1 = name;
	}

	//--------------------------------------------------------------
	void setFontTitleSize(int size = 10) { // Call before setup. Default it's 10
		size_TTF2 = size;
	}
	//--------------------------------------------------------------
	void setFontTitleName(string name) { // Set the name only. path is /assets/fonts/*name*
		name_TTF2 = name;
	}

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

		buildFonts();
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