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
using namespace varx;

#include "E:/projucer/JUCE/modules/varx/varx/rxcpp/Rx/v2/src/rxcpp/rx.hpp"
namespace Rx {
	using namespace rxcpp;
	using namespace rxcpp::sources;
	using namespace rxcpp::operators;
	using namespace rxcpp::util;
}
using namespace Rx;

//[/Headers]

#include "DrawComponent.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class RxComponent  : public AudioAppComponent,
                     public ChangeListener,
                     public Timer,
                     public SliderListener,
                     public ButtonListener
{
public:
    //==============================================================================
    RxComponent ( );
    ~RxComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	//PSMixer::PSAudioSource* audio_source_01 = nullptr;
	//PSMixer::PSAudioSource* audio_source_02 = nullptr;
	//PSMixer::PSAudioSource* audio_source_03 = nullptr;

	rxcpp::composite_subscription sth, sth2;


	bool keyPressed(const KeyPress& key) override
	{
	//	msg("key press");
		// need improve 
		if (key.isKeyCode('0'))
		{
			drum.audio_source_type_0->setPosition(drum.audio_source_type_0->_startTime);
			drum.audio_source_type_0->setGain(1.0);
			drum.audio_source_type_0->start();

		}
		else if (key.isKeyCode('1'))
		{
			drum.audio_source_type_1->setPosition(drum.audio_source_type_1->_startTime);
			drum.audio_source_type_1->setGain(1.0);
			drum.audio_source_type_1->start();

		}
		return true;
	}

	void drawAt( bool inThread = true) 
	{
		juce::Array<units> ary;
		drum.getUnitToDraw(ary);
		{
			const juce::MessageManagerLock mml(Thread::getCurrentThread());
			if (inThread)
				if (mml.lockWasGained())
					component->drawCircleAt(ary);
		}


		//int nxttime = drum.getCurTypeTimeByIndex(drum.getIndex(0));

		//float difference = nxttime - g_pos;
		//difference = difference / drum._window;

		//float difference = ary.getReference(0).timeDifference;

		//if (difference >= 0.0f && difference <= 1.0f)
		//{
		//	const juce::MessageManagerLock mml(Thread::getCurrentThread());
		//	if(inThread)
		//		if (mml.lockWasGained())
		//			component->drawCircleAt(difference, 0.5);
		//}

	}
	void startMixer()
	{
	//	//PSMixer::DeviceManager deviceManager; 
	//	ScopedPointer<PSMixer::DeviceManager>  deviceManager = new 	PSMixer::DeviceManager;
	//	File file_01("C:/Users/zhufei/Desktop/Dennis Kuo - Sweet Saturn.mp3");
	//	//File file_02("C:/Users/zhufei/Desktop/mp3/dusk.mp3");
	//	File file_02("C:/Users/zhufei/Desktop/Dennis Kuo - Sweet Saturn_type12.mp3");
	//	File file_03("C:/Users/zhufei/Desktop/mp3/killer.mp3");
	//	audio_source_01 = new PSMixer::PSAudioSource;
	//	audio_source_01->setAudioFormatReader(deviceManager->getAudioFormatReader(file_01));


	//	audio_source_02 = new PSMixer::PSAudioSource;
	//	audio_source_02->setAudioFormatReader(deviceManager->getAudioFormatReader(file_02));


	//	audio_source_03 = new PSMixer::PSAudioSource;
	//	audio_source_03->setAudioFormatReader(deviceManager->getAudioFormatReader(file_03));/**/

	//	PSMixer::Mixer *mixer = new PSMixer::Mixer;
	//	mixer->addStem(audio_source_01, true);
	//	mixer->addStem(audio_source_02, 0);
	//	mixer->addStem(audio_source_03, 1);


	//	deviceManager->setMixer(mixer);

	//	audio_source_01->setGain(1.0);
	//	audio_source_02->setGain(1.0);
	//	audio_source_03->setGain(1.0);

	//	audio_source_01->start();
	//	audio_source_02->start();
	//	//audio_source_03->start();

	//	AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "getchar", "getchar", "getchar");

	////	audio_source_02->stop();
	////	deviceManager->resetMixer();

	//	AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "getchar", "getchar", "getchar");


		//audio_source_01->stop();
		//audio_source_02->stop();
		//AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "getchar", "getchar", "getchar");
		//mixer.releaseResources();

		//audio_source_01.setGain(1.0);
		//audio_source_02.setGain(1.0);
		//audio_source_03.setGain(1.0);
	}


	void open()
	{
		FileChooser chooser("Select a Wave file to play...",
			File::nonexistent,
			"*.*");

		if (chooser.browseForFileToOpen())
		{
			File file(chooser.getResult());
			AudioFormatReader* reader = formatManager.createReaderFor(file);

			if (reader != nullptr)
			{
				ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource(reader, true);
				audioTransportSource.setSource(newSource, 0, nullptr, reader->sampleRate);
				audioReaderSource = newSource.release();
				state->rx.subject.onNext(AudioTransportState::Stopped);


			}
		}

	}

	void openmix()
	{
		{
			FileChooser chooser("Select a Wave file to play...",
				File::nonexistent,
				"*.*");

			if (chooser.browseForFileToOpen())
			{
				File file(chooser.getResult());
				AudioFormatReader* reader = formatManager.createReaderFor(file);

				if (reader != nullptr)
				{

					ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource(reader, true);
					audioTransportSource.setSource(newSource, 0, nullptr, reader->sampleRate);
					audioReaderSource = newSource.release();


					//juce::AudioTransportSource * PaudioTransportSource = new AudioTransportSource();
					//PaudioTransportSource->setSource(new AudioFormatReaderSource(reader, true));


					//mixerSource.addInputSource(new AudioFormatReaderSource(reader, true), true);
					//PaudioTransportSource->start();

				}
			}

		}


		{
			FileChooser chooser("Select a Wave file to play...",
				File::nonexistent,
				"*.*");

			if (chooser.browseForFileToOpen())
			{
				File file(chooser.getResult());
				AudioFormatReader* reader = formatManager.createReaderFor(file);

				if (reader != nullptr)
				{

					ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource(reader, true);
					audioTransportSource.setSource(newSource, 0, nullptr, reader->sampleRate);
					audioReaderSource = newSource.release();


					//juce::AudioTransportSource * PaudioTransportSource = new AudioTransportSource;
					//PaudioTransportSource->setSource(new AudioFormatReaderSource(reader, true));

					//	PaudioTransportSource->start();

					//mixerSource.addInputSource(new AudioFormatReaderSource(reader, true), true);

				}
			}

		}

		//audioTransportSource.setSource(mixerSource.);

		//mixerSource.prepareToPlay(512, 48000.0);

		//	AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "title", "you cancel the file choose dialog", "exit");

	}



	void timerCallback() override
	{
		//AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "timerCallback", "timerCallback", "getchar");

	//	if(audio_source_01)
		{
		//	label->rx.text.onNext(juce::String(audio_source_01->getCurrentPosition()));
		}
	}


	void changeListenerCallback(ChangeBroadcaster* source) override
	{
		if (source == &audioTransportSource)
		{
			if (audioTransportSource.isPlaying())
			{
				state->rx.subject.onNext(AudioTransportState::Playing);
			}
			else
			{
				int cur = state->getValue();
				if (cur == (int)AudioTransportState::Stopping ||
					cur == (int)AudioTransportState::Playing
					)
				{
					state->rx.subject.onNext(AudioTransportState::Stopped);
				}
				else if (cur == (int)AudioTransportState::Pausing)
				{
					state->rx.subject.onNext(AudioTransportState::Paused);
				}

			}


			//	changeAudioTransportState(Playing);
			//else if ((audioTransportState == Stopping) || (audioTransportState == Playing))
			//	changeAudioTransportState(Stopped);
			//else if (audioTransportState == Pausing)
			//	changeAudioTransportState(Paused);
		}
	}


	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{
		// Setup Audio Source
			audioTransportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	}


	void releaseResources() override
	{
		audioTransportSource.releaseResources();
	}




	/** The audio rendering callback.
	*/
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		if (audioReaderSource == nullptr)
		{
			bufferToFill.clearActiveBufferRegion();
			return;
		}

		audioTransportSource.getNextAudioBlock(bufferToFill);

		// Write to Ring Buffer
		int numChannels = bufferToFill.buffer->getNumChannels();
		for (int i = 0; i < numChannels; ++i)
		{
			const float * audioData = bufferToFill.buffer->getReadPointer(i, bufferToFill.startSample);
			//  ringBuffer->writeSamples (audioData, bufferToFill.numSamples, i);
		}

		//double pos = audio_source_01->getCurrentPosition();
		//label->rx.text.onNext(juce::String(pos));
	}



    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	Drum drum;
	ScopedPointer<Reactive<Slider>> slider;
	ScopedPointer<Reactive<Label>> label;
	ScopedPointer<Reactive<TextButton>> openButton;
	ScopedPointer<Reactive<TextButton>> playButton;
	ScopedPointer<Reactive<TextButton>> stopButton;

	enum AudioTransportState
	{
		init ,
		Stopped,
		Starting,
		Playing,
		Pausing,
		Paused,
		Stopping
	};


	ScopedPointer<Reactive<Value>> state;

	varx::Observable* values;


	juce::AudioFormatManager formatManager;
	ScopedPointer<juce::AudioFormatReaderSource> audioReaderSource;
	juce::AudioTransportSource audioTransportSource;

	juce::MixerAudioSource mixerSource;

    //[/UserVariables]

    //==============================================================================
    //ScopedPointer<Slider> slider;
    //ScopedPointer<Label> label;
    //ScopedPointer<TextButton> playButton;
    //ScopedPointer<TextButton> stopButton;
    //ScopedPointer<TextButton> openButton;
    ScopedPointer<DrawComponent> component;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RxComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
