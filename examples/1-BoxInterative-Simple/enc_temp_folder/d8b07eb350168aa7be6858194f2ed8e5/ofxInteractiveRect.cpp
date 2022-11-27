//
//  interactiveRect.cpp
//
//  Created by Roy Macdonald on 8/15/12.
//  Copyright (c) 2012 micasa. All rights reserved.
//
//  Updated by Aaron Richards on 13/05/2014.
//	Updated by moebiusSurfing 2021

#include "ofxInteractiveRect.h"

//--------------------------------------------------------------
ofxInteractiveRect::ofxInteractiveRect(string name, string path)
{
	bEditMode.addListener(this, &ofxInteractiveRect::Changed_EditMode);
	rectParam.addListener(this, &ofxInteractiveRect::Changed_Rect);

	bLockResize = false;
	bLock = false;

	bIsEditing = false;
	bMove = false;
	bLeft = false;
	bRight = false;
	bUp = false;
	bDown = false;
	bIsOver = false;
	this->name = name;
	//this->path = "";

	setRect(10, 10, 400, 400);
	if (bAutoSave) loadSettings();

	// Some grey coloring
	setColorBorderDraggable(ofColor(0, 128));
	setColorEditingPressedBorder(ofColor(0, 128));
	setColorEditingMoving(ofColor(128, 32));

	refreshConstraints();
}

//--------------------------------------------------------------
ofxInteractiveRect::~ofxInteractiveRect()
{
	bEditMode.removeListener(this, &ofxInteractiveRect::Changed_EditMode);
	rectParam.removeListener(this, &ofxInteractiveRect::Changed_Rect);

	if (bAutoSave) saveSettings();
}

//--------------------------------------------------------------
void ofxInteractiveRect::enableEdit(bool enable)
{
	ofLogVerbose("ofxInteractiveRect") << " " << (__FUNCTION__) << "Rect " << this->name << " edit : " << (string)(enable ? "true" : "false");

	if (enable != bIsEditing)
	{
		if (enable)
		{
			ofRegisterMouseEvents(this);
		}
		else
		{
			ofUnregisterMouseEvents(this);
			//saveSettings();
		}
		bIsEditing = enable;

		bEditMode.setWithoutEventNotifications(bIsEditing);
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::disableEdit()
{
	enableEdit(false);
}

//--------------------------------------------------------------
void ofxInteractiveRect::toggleEdit()
{
	enableEdit(!bIsEditing);
}

//--------------------------------------------------------------
ofRectangle ofxInteractiveRect::getRect()
{
	return ofRectangle(x, y, width, height);
}

//--------------------------------------------------------------
void ofxInteractiveRect::saveSettings(string name, string path, bool saveJson)
{
	if (name != "")
	{
		this->name = name;
	}

	if (path != "")
	{
		this->path = path;
	}

	//string filename = this->path + prefixName + this->name;
	string filename = this->path + this->name + "_" + prefixName;

	if (saveJson) {
		filename += ".json";
		ofSaveJson(filename, saveToJson());
	}
	else {
		filename += ".xml";
		saveToXml().save(filename);
	}

	ofLogNotice("ofxInteractiveRect") << " " << (__FUNCTION__) << filename;
}

//--------------------------------------------------------------
ofJson ofxInteractiveRect::saveToJson()
{
	ofLogNotice("ofxInteractiveRect") << " " << (__FUNCTION__);

	ofJson j;//("interactiveRect");

	j["x"] = this->ofRectangle::x;
	j["y"] = this->ofRectangle::y;
	j["width"] = this->ofRectangle::width;
	j["height"] = this->ofRectangle::height;
	j["name"] = this->name;
	j["path"] = this->path;
	j["isEditing"] = this->bIsEditing;
	return j;
}

//--------------------------------------------------------------
void ofxInteractiveRect::loadFromJson(const ofJson& j)
{
	ofLogNotice("ofxInteractiveRect") << " " << (__FUNCTION__);

	if (j == nullptr) return;//TODO: crash

	j["x"].get_to(this->ofRectangle::x);
	j["y"].get_to(this->ofRectangle::y);
	j["width"].get_to(this->ofRectangle::width);
	j["height"].get_to(this->ofRectangle::height);
	j["name"].get_to(this->name);
	j["path"].get_to(this->path);
	bool editing;

	j["isEditing"].get_to(editing);
	enableEdit(editing);

	// constraint
	refreshConstraints();
}

//--------------------------------------------------------------
ofXml ofxInteractiveRect::saveToXml()
{
	ofXml xml;
	auto r = xml.appendChild("interactiveRect");

	r.appendChild("path").set(this->path);
	r.appendChild("x").set(this->ofRectangle::x);
	r.appendChild("y").set(this->ofRectangle::y);
	r.appendChild("width").set(this->ofRectangle::width);
	r.appendChild("height").set(this->ofRectangle::height);
	r.appendChild("name").set(this->name);
	r.appendChild("isEditing").set(this->bIsEditing);

	return xml;
}

//--------------------------------------------------------------
bool ofxInteractiveRect::loadFromXml(const ofXml& xml)
{
	auto r = xml.getChild("interactiveRect");
	if (r)
	{
		this->path = r.getChild("path").getValue();
		this->ofRectangle::x = r.getChild("x").getFloatValue();
		this->ofRectangle::y = r.getChild("y").getFloatValue();
		this->ofRectangle::width = r.getChild("width").getFloatValue();
		this->ofRectangle::height = r.getChild("height").getFloatValue();
		this->name = r.getChild("name").getValue();
		enableEdit(r.getChild("isEditing").getBoolValue());

		// constraint
		refreshConstraints();

		return true;
	}

	return false;
}

//--------------------------------------------------------------
bool ofxInteractiveRect::loadSettings(string name, string path, bool loadJson)
{
	if (name != "")
	{
		this->name = name;
	}
	if (path != "")
	{
		this->path = path;
	}

	//string filename = this->path + prefixName + this->name;
	string filename = this->path + this->name + "_" + prefixName;

	if (loadJson)
	{
		filename += ".json";

		//avoid crash
		ofFile file;
		file.open(filename);
		if (!file.exists()) return false;

		loadFromJson(ofLoadJson(filename));
		return true;

	}
	else
	{
		filename += ".xml";

		//avoid crash
		ofFile file;
		file.open(filename);
		if (!file.exists()) return false;

		ofXml xml;
		if (xml.load(filename))
		{
			if (loadFromXml(xml))
			{
				return true;
			}
		}
	}

	ofLogVerbose("ofxInteractiveRect") << " " << (__FUNCTION__) << "unable to load : " << filename;
	rectParam.set(this->getRect());//?

	refreshConstraints();

	return false;
}

//TODO:
//--------------------------------------------------------------
void ofxInteractiveRect::drawBorder()
{
	if (bTransparent) return;

	//--

	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(2.0);
	if (bEditMode) ofSetColor(ofColor(colorEditingMoving.r, colorEditingMoving.g, colorEditingMoving.b, colorEditingMoving.a * Bounce()));
	else ofSetColor(colorEditingMoving);
	if (bRounded) ofDrawRectRounded(*this, rounded);
	else ofDrawRectangle(*this);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofxInteractiveRect::draw()
{
	if (bTransparent) return;

	//--

	if (bIsEditing)
	{
		ofPushStyle();

		// Hover

		if (bIsOver)
		{
			// Border

			ofNoFill();
			if (!bPressed)
			{
				//ofSetColor(ofColor(ofColor::orange, 128));
				ofSetColor(colorEditingHover);
			}
			else
			{
				//ofSetColor(ofColor(ofColor::red, 128));
				ofSetColor(colorEditingPressedBorder);
			}

			if (bRounded) ofDrawRectRounded(*this, rounded);
			else ofDrawRectangle(*this);
		}

		// Border

		//else 
		{
			drawBorder(); // will blink
		}

		//--

		// Fill

		ofFill();

		bool bDual = (bUp || bDown) && (bLeft || bRight);

		if (bMove) // when moving
		{
			ofSetColor(colorEditingMoving);

			if (bRounded) ofDrawRectRounded(*this, rounded);
			else ofDrawRectangle(*this);
		}
		else // when static
		{
			// Draggable borders

			ofSetColor(colorBorderDraggable.r, colorBorderDraggable.g, colorBorderDraggable.b, colorBorderDraggable.a * 0.5);

			float xx, yy, ww, hh;

			// height
			if (bUp)
			{
				//x,y,width, here are "inherited" from ofRectangle
				ww = width;
				hh = height;
				xx = x;
				yy = y;

				//if (bDual) 
				//{
				//	if (bLeft) xx = x + BORDER_DRAG_SIZE;
				//	else if(bRight) ww = ww - BORDER_DRAG_SIZE;
				//}

				if (bRounded) ofDrawRectRounded(xx, yy, ww, BORDER_DRAG_SIZE, rounded);
				else ofDrawRectangle(xx, yy, ww, BORDER_DRAG_SIZE);
			}
			else if (bDown)
			{
				ww = width;
				hh = height;
				xx = x;
				yy = y + hh - BORDER_DRAG_SIZE;

				//if (bDual)
				//{
				//	if (bLeft) xx = x + BORDER_DRAG_SIZE;
				//	else if (bRight) ww = ww - BORDER_DRAG_SIZE;
				//}

				if (bRounded) ofDrawRectRounded(xx, yy, ww, BORDER_DRAG_SIZE, rounded);
				else ofDrawRectangle(xx, yy, ww, BORDER_DRAG_SIZE);
			}

			// width
			if (bLeft)
			{
				ww = width;
				hh = height;
				xx = x;
				yy = y;

				if (bDual)
				{
					if (bUp) {
						yy = y + BORDER_DRAG_SIZE;
						hh = height - BORDER_DRAG_SIZE;
					}
					else if (bDown) {
						yy = y;
						hh = height - BORDER_DRAG_SIZE;
					}
				}

				if (bRounded) ofDrawRectRounded(xx, yy, BORDER_DRAG_SIZE, hh, rounded);
				else ofDrawRectangle(xx, yy, BORDER_DRAG_SIZE, hh);
			}
			else if (bRight)
			{
				ww = width;
				hh = height;
				xx = x + width - BORDER_DRAG_SIZE;
				yy = y;

				if (bDual)
				{
					if (bUp) {
						yy = y + BORDER_DRAG_SIZE;
						hh = height - BORDER_DRAG_SIZE;
					}
					else if (bDown) {
						yy = y;
						hh = height - BORDER_DRAG_SIZE;
					}
				}

				if (bRounded) ofDrawRectRounded(xx, yy, BORDER_DRAG_SIZE, hh, rounded);
				else ofDrawRectangle(xx, yy, BORDER_DRAG_SIZE, hh);
			}
		}

		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseMoved(ofMouseEventArgs& mouse)
{
	if (bLock) return;

	if (!bPressed)
	{
		bIsOver = inside(mouse.x, mouse.y);

		bLeft = false;
		bRight = false;
		bUp = false;
		bDown = false;

		if (bIsOver)
		{
			bMove = true;

			if (!bLockResize)
			{
				if (mouse.x < x + BORDER_DRAG_SIZE && mouse.x > x && !bLockW)
				{
					bLeft = true;
					bMove = false;
				}
				else if (mouse.x < x + width && mouse.x > x + width - BORDER_DRAG_SIZE && !bLockW)
				{
					bRight = true;
					bMove = false;
				}

				if (mouse.y > y && mouse.y < y + BORDER_DRAG_SIZE && !bLockH)
				{
					bUp = true;
					bMove = false;
				}
				else if (mouse.y > y + height - BORDER_DRAG_SIZE && mouse.y < y + height && !bLockH)
				{
					bDown = true;
					bMove = false;
				}
			}
		}
		else
		{
			bMove = false;
		}
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::mousePressed(ofMouseEventArgs& mouse)
{
	if (bLock) return;
	if (!bEditMode) return;
	if (!this->isMouseOver()) return;

	mousePrev = mouse;
	bPressed = true;

	if (!bAllScreenMouse) bIsOver = inside(mouse.x, mouse.y);
	else bIsOver = true;

	bLeft = false;
	bRight = false;
	bUp = false;
	bDown = false;

	if (bIsOver)
	{
		bMove = true;

		if (!bLockResize)
		{
			if (mouse.x < x + BORDER_DRAG_SIZE && mouse.x > x && !bLockW)
			{
				bLeft = true;
				bMove = false;
			}
			else if (mouse.x < x + width && mouse.x > x + width - BORDER_DRAG_SIZE && !bLockW)
			{
				bRight = true;
				bMove = false;
			}
			if (mouse.y > y && mouse.y < y + BORDER_DRAG_SIZE && !bLockH)
			{
				bUp = true;
				bMove = false;
			}
			else if (mouse.y > y + height - BORDER_DRAG_SIZE && mouse.y < y + height && !bLockH)
			{
				bDown = true;
				bMove = false;
			}
		}
	}
	else
	{
		bMove = false;
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseDragged(ofMouseEventArgs& mouse)
{
	if (bLock) return;
	if (!bEditMode) return;
	//if (!this->isMouseOver()) return;

	float diffx = mouse.x - mousePrev.x;
	float diffy = mouse.y - mousePrev.y;

	//if (!bLockResize) 
	{
		if (bUp && !bLockY)
		{
			//diffx = ofClamp(diffx, pad, ofGetHeight() - pad);
			y += diffy;
			y = ofClamp(y, pad, ofGetHeight() - pad);
			height += diffy;

			if (bLockAspectRatio) width = aspectRatio * height;
		}
		else if (bDown && !bLockH)
		{
			height += diffy;
			height = MIN(height, ofGetHeight() - y - 2 * pad);

			if (bLockAspectRatio) width = aspectRatio * height;
		}

		if (bLeft && !bLockX)
		{
			x += diffx;
			x = ofClamp(x, pad, ofGetWidth() - pad);
			width += diffx;

			if (bLockAspectRatio) height = width / aspectRatio;
		}
		else if (bRight && !bLockW)
		{
			width += diffx;
			width = MIN(width, ofGetWidth() - x - 2 * pad);

			if (bLockAspectRatio) height = width / aspectRatio;
		}
	}

	if (bMove)
	{
		x += diffx;
		y += diffy;

		//x = ofClamp(x, pad, ofGetWidth() - pad);
		//y = ofClamp(y, pad, ofGetHeight() - pad);
	}

	// constraint
	refreshConstraints();

	mousePrev = mouse;
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseReleased(ofMouseEventArgs& mouse)
{
	if (bLock) return;
	if (!bEditMode) return;
	//if (!this->isMouseOver()) return;

	//if (!bLockResize) 
	{
		bLeft = false;
		bRight = false;
		bUp = false;
		bDown = false;
	}
	bMove = false;
	bPressed = false;

	// clamp inside the window
	int _min = 20;//min size
	width = ofClamp(width, _min, ofGetWidth());
	height = ofClamp(height, _min, ofGetHeight());

	width = MIN(width, ofGetWidth() - x - 2 * pad);
	height = MIN(height, ofGetHeight() - y - 2 * pad);

	// constraint
	refreshConstraints();

	rectParam.setWithoutEventNotifications(this->getRect());
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseScrolled(ofMouseEventArgs& mouse) {
	if (!bEnableMouseWheel) return;
	if (!bEditMode) return;
	if (!this->isMouseOver()) return;
	if (bLockResize) return;

	//if (width <= shapeConstraintMin.x + 20 || height <= shapeConstraintMin.y + 20) {
	//	return;
	//	// skip to avoid move
	//}

	//glm::vec2 p = glm::vec2(mouse.x, mouse.y);
	//ofLogNotice("ofxInteractiveRect")<<(__FUNCTION__) << mouse.scrollY;

	float d = 0.1f;
	float s = ofMap(mouse.scrollY, -2, 2, 1.f - d, 1.f + d);

	bool bKeyModShift = ofGetKeyPressed(OF_KEY_LEFT_SHIFT);
	bool bKeyModCtrl = ofGetKeyPressed(OF_KEY_CONTROL);
	bool bKeyModAlt = ofGetKeyPressed(OF_KEY_ALT);

	if (bKeyModCtrl && !bLockW && !bLockH) this->scaleFromCenter(s);
	else if (bKeyModShift && !bLockW) this->scale(s, 1);
	else if (bKeyModAlt && !bLockH) this->scale(1, s);
	else {
		if (!bLockW) this->scale(s, 1);
		if (!bLockH) this->scale(1, s);
	}

	if (bLockAspectRatio) height = width / aspectRatio;

	// constraint
	refreshConstraints();
}

//--------------------------------------------------------------
void ofxInteractiveRect::refreshConstraints() {

	// apply constraints if defined
	if (bConstrainedMin)
	{
		this->width = MAX(width, shapeConstraintMin.x);
		this->height = MAX(height, shapeConstraintMin.y);
	}
	if (bConstrainedMax)
	{
		this->width = MIN(width, shapeConstraintMax.x);
		this->height = MIN(height, shapeConstraintMax.y);
	}

	//// clamp pad to borders
	//x = ofClamp(x, pad, ofGetWidth() - pad - width);
	//y = ofClamp(y, pad, ofGetHeight() - pad - height);

	//width = MIN(width, ofGetWidth() - x - 2 * pad);
	//height = MIN(height, ofGetHeight() - y - 2 * pad);
}

void ofxInteractiveRect::mouseEntered(ofMouseEventArgs& mouse) {}
void ofxInteractiveRect::mouseExited(ofMouseEventArgs& mouse) {}

//--------------------------------------------------------------
void ofxInteractiveRect::Changed_EditMode(bool& b)
{
	ofLogNotice("ofxInteractiveRect") << " " << (__FUNCTION__) << b;

	enableEdit(b);
}

//--------------------------------------------------------------
void ofxInteractiveRect::Changed_Rect(ofRectangle& r)
{
	ofLogNotice("ofxInteractiveRect") << " " << (__FUNCTION__) << r;

	this->set(r);

	refreshConstraints();
}