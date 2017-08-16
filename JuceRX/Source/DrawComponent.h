/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "Mixer.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class DrawComponent  : public Component
{
public:
    //==============================================================================
    DrawComponent ();
    ~DrawComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	void drawCircleAt(juce::Array<units> &ary)
	{
		_unitsToDraw = ary;
		repaint();
	}
	//void drawCircleAt(float percentagew, float percentageh, int width = 10, int height = 10)
	//{
	//	//float wp = getWidth()*percentagew;
	//	//float hp = getHeight() * percentageh;
	//	//g.fillEllipse(wp, hp, width, height);
	//	_pw = getWidth()*percentagew;
	//	_ph = getHeight() *percentageh;
	//	_w = width;
	//	_h = height;
	//	repaint();
	//}

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	float _pw, _ph;
	int _w, _h;
	juce::Array<units> _unitsToDraw;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrawComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
