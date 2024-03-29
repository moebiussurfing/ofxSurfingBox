# ofxSurfingBox

## Overview
This add-on has three classes: 
1. A draggable rectangle: **ofxSurfingBoxInteractive** 
2. A draggable rectangle with text info: **ofxSurfingBoxHelpText**
3. Same but with `Fontstash` with paragraph formatting with tag styles: **ofxSurfingBoxParagraph**

## Screenshots
![](/examples/2-BoxHelpText-Simple/Capture.PNG)
![](/examples/3-BoxHelpText-Debug/Capture.PNG)
![](/examples/5-BoxTextParagraph/Capture.PNG)

## Features

### ofxSurfingBoxInteractive.h
- This class is like a draggable ofRectangle.
- It can be used as a viewport for a camera, a videoplayer, or like any ofRectangle.
- Can be placed into top, bottom, center, left... positions.
- Also can be positioned freely and scaled manually.
- Mouse wheel modifies the scale.
- Aspect ratio can be locked.
- Double click activates Editing or Locked modes: 
   * to allow move the position and resize using the mouse.
- Auto stores and recall the settings between sessions.

### ofxSurfingBoxHelpText.h
- This class draws the same, a draggable box with a filled background but with text.
- Useful to display **Help Info** boxes in your apps.
- Tittle font for the first text lines.

### ofxSurfingBoxParagraph.h
- Same as `ofxSurfingBoxHelpText` with powerful tag styling.

## Usage
- See the examples.

## Dependencies
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxFontStash2](https://github.com/moebiussurfing/ofxFontStash2) / Only for using ofxSurfingBoxParagraph.

## Third Party add-ons bundled into **ofxSurfingHelpers**
Already added to `OFX_ADDON/libs/`. No need to add to **PROJECT GENERATOR**.  
* [ofxScaleDragRect](https://github.com/moebiussurfing/ofxScaleDragRect) / _Fork from [roymacdonald](https://github.com/roymacdonald/ofxScaleDragRect)_  
* [ofxMSAInteractiveObject](https://github.com/moebiussurfing/ofxMSAInteractiveObject) / _Fork from [memo](https://github.com/memo/ofxMSAInteractiveObject)_  

## Tested System
* **Windows 10** / **VS 2022** / **OF 0.12+**

## Author
An add-on by **@moebiusSurfing**  
*( ManuMolina ) 2019-2022*  

## License
**MIT License**
