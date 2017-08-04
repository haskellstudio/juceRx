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

//[Headers] You can add your own extra header files here...
#include <tuple>

using namespace std;
//[/Headers]

#include "RxComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...


#include "E:/projucer/JUCE/modules/varx/varx/rxcpp/Rx/v2/src/rxcpp/rx.hpp"
namespace Rx {
	using namespace rxcpp;
	using namespace rxcpp::sources;
	using namespace rxcpp::operators;
	using namespace rxcpp::util;
}
using namespace Rx;

static String textForDistortion(double distortion) {
	return (distortion < 5 ? "cold" : "hot!");
}



//[/MiscUserDefs]

//==============================================================================
RxComponent::RxComponent ( )
{
    //[Constructor_pre] You can add your own custom stuff here..
	//Reactive<TextButton> button("Click Here");

	formatManager.registerBasicFormats();
	setAudioChannels(2, 2);
	audioTransportSource.addChangeListener(this);

	state = new Reactive<Value>((int64)0);


	addAndMakeVisible(slider = new Reactive<Slider>("new slider"));
	addAndMakeVisible(label = new Reactive<Label>("new label", TRANS("label text")));
	addAndMakeVisible(playButton = new Reactive<TextButton>("playButton"));
	addAndMakeVisible(stopButton = new Reactive<TextButton>("stopButton"));
	addAndMakeVisible(openButton = new Reactive<TextButton>("openButton"));
	//Observable text = slider->rx.value.map([](double distortion) { return (distortion < 5 ? "cold" : "hot!"); });
	//text.subscribe(label->rx.text);








    //[/Constructor_pre]

   // addAndMakeVisible (slider = new Slider ("new slider"));
    slider->setRange (0, 10, 0);
    slider->setSliderStyle (Slider::LinearHorizontal);
    slider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider->addListener (this);

   // addAndMakeVisible (label = new Label ("new label",
   //                                       TRANS("label text")));
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

  //  addAndMakeVisible (playButton = new TextButton ("playButton"));
    playButton->setButtonText (TRANS("new button"));
    playButton->addListener (this);

  //  addAndMakeVisible (stopButton = new TextButton ("stopButton"));
    stopButton->setButtonText (TRANS("new button"));
    stopButton->addListener (this);

  //  addAndMakeVisible (openButton = new TextButton ("openButton"));
    openButton->setButtonText (TRANS("new button"));
    openButton->addListener (this);


    //[UserPreSize]

    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	//slider->rx.value.map([](double distortion) { return (distortion < 5 ? "cold" : "hot!"); }).subscribe(label->rx.text);
	openButton->rx.clicked.subscribe(
		[this](int i) {
						//openmix();
						/* open();*/
		startMixer();
		//Drum drum;

		//drum.ajust(1.6);
		//int index = drum.getIndex(1);
		//AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, juce::String(index), "index", "ok");
		
		}
	);



	playButton->rx.clicked.subscribe(
		[this](juce::String i) {
		int cur = state->getValue();

		if (cur == (int)AudioTransportState::Stopped ||
			cur == (int)AudioTransportState::Paused)
		{
			state->rx.subject.onNext(AudioTransportState::Starting);
		}

		if (cur == (int)AudioTransportState::Playing)
		{
			state->rx.subject.onNext(AudioTransportState::Pausing);
		}
	}
	);

	stopButton->rx.clicked.subscribe(
		[this](int i) {
		int cur = state->getValue();

		if (cur == (int)AudioTransportState::Paused)
		{
			state->rx.subject.onNext(AudioTransportState::Stopped);
		}
		else
		{
			state->rx.subject.onNext(AudioTransportState::Stopping);
		}
	});



	state->rx.subject
		.scan(
							(int64)0,
							[](int64 seed, int v) {  return  seed << 32 | v; })
		.subscribe(
							[this](int64 stateTuple) {  AudioTransportState oldState = (AudioTransportState)(int(stateTuple >> 32));

														AudioTransportState newState = (AudioTransportState)(int(stateTuple)) ;


														if (oldState == AudioTransportState::init && newState == AudioTransportState::init) {   // the init state
															playButton->setButtonText("play");
															playButton->setEnabled(false);

															stopButton->setButtonText("stop");
															stopButton->setEnabled(false);

															openButton->setButtonText("open");
														}

														if ( newState == AudioTransportState::Starting) { // user start play the song
															audioTransportSource.start();
														}


														if (newState == AudioTransportState::Stopping || newState == AudioTransportState::Pausing) {  // user pause the song
															audioTransportSource.stop();
														}


														if (newState == AudioTransportState::Playing) {   // song is playing

															playButton->setButtonText("Pause");
															stopButton->setButtonText("Stop");
															playButton->setEnabled(true);
															stopButton->setEnabled(true);
														}


														if (newState == AudioTransportState::Paused) {   // song is Paused

															playButton->setButtonText("Resume");
															stopButton->setButtonText("Return to Zero");
															playButton->setEnabled(true);
															stopButton->setEnabled(true);
														}


														if (newState == AudioTransportState::Stopped) {   // song is Stopped

															playButton->setButtonText("Play");
															stopButton->setButtonText("Stop");
															playButton->setEnabled(true);
															stopButton->setEnabled(false);
															audioTransportSource.setPosition(0.0);
														}

													}
		);



	//Observable::interval(RelativeTime::seconds(3)).subscribe([this](int i) {
	//	//if(audio_source_01)
	//		//label->rx.text.onNext(juce::String(audio_source_01->getCurrentPosition()));
	//																				});


	auto s1 = rxcpp::observe_on_new_thread();
	//auto s2 = rxcpp::identity_current_thread();
	//auto start = s1.now() + std::chrono::seconds(1);
	auto period = std::chrono::seconds(1);
	auto values = rxcpp::observable<>::interval(period, s1);
	values.
		//take(3).
		subscribe(
			[this](int v) { 
								const juce::MessageManagerLock mml(Thread::getCurrentThread());
								if (!mml.lockWasGained())  // if something is trying to kill this job, the lock
									return;
								if(audio_source_01)
									label->rx.text.onNext(juce::String(audio_source_01->getCurrentPosition()));

						});


	slider->rx.value.map(&textForDistortion).subscribe(label->rx.text);


	//startTimer(1000);
    //[/Constructor]
}

RxComponent::~RxComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

	state = nullptr;

    //[/Destructor_pre]

    slider = nullptr;
    label = nullptr;
    playButton = nullptr;
    stopButton = nullptr;
    openButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	shutdownAudio();
    //[/Destructor]
}

//==============================================================================
void RxComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void RxComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    slider->setBounds (144, 144, 496, 72);
    label->setBounds (144, 352, 480, 24);
    playButton->setBounds (144, 96, 150, 24);
    stopButton->setBounds (352, 96, 150, 24);
    openButton->setBounds (144, 24, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void RxComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider)
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
        //[/UserSliderCode_slider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void RxComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == playButton)
    {
        //[UserButtonCode_playButton] -- add your button handler code here..
        //[/UserButtonCode_playButton]
    }
    else if (buttonThatWasClicked == stopButton)
    {
        //[UserButtonCode_stopButton] -- add your button handler code here..
        //[/UserButtonCode_stopButton]
    }
    else if (buttonThatWasClicked == openButton)
    {
        //[UserButtonCode_openButton] -- add your button handler code here..
        //[/UserButtonCode_openButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="RxComponent" componentName=""
                 parentClasses="public AudioAppComponent, public ChangeListener, public Timer"
                 constructorParams=" " variableInitialisers=" " snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <SLIDER name="new slider" id="aa599092a6eb17ee" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="144 144 496 72" min="0" max="10"
          int="0" style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="new label" id="f38f17ebb5550d29" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="144 352 480 24" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="playButton" id="7ce8c593eeb3cb6c" memberName="playButton"
              virtualName="" explicitFocusOrder="0" pos="144 96 150 24" buttonText="new button"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="stopButton" id="cdd63ed5eef7e029" memberName="stopButton"
              virtualName="" explicitFocusOrder="0" pos="352 96 150 24" buttonText="new button"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="openButton" id="8ec229f8f23cddcb" memberName="openButton"
              virtualName="" explicitFocusOrder="0" pos="144 24 150 24" buttonText="new button"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
