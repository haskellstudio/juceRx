/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "RxComponent.h"




class Som : public AudioAppComponent,
	public Button::Listener
{
public:
	//==============================================================================
	Som()
	{
		formatManager.registerBasicFormats();
		setAudioChannels(2, 2); 

								// Setup GUI
		addAndMakeVisible(&openButton);
		openButton.setButtonText("Open...");
		openButton.addListener(this);
		setSize(800, 600); // Set Component Size
	}

	~Som()
	{
		shutdownAudio();
	}

	//==============================================================================
	// Audio Callbacks

	/** Called before rendering Audio.
	*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{
		// Setup Audio Source
		audioTransportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

	}

	/** Called after rendering Audio.
	*/
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
	}


	void paint(Graphics& g) override
	{
		g.fillAll(Colour(0xFF252831)); // Set background color (below any GL Visualizers)
	}


	void resized() override
	{
		const int w = getWidth();
		const int h = getHeight();

		// Button Width
		int buttonWidth = (w - 30) / 2;

		openButton.setBounds(10, 10, buttonWidth, 20);

	}



	void buttonClicked(Button* button) override
	{
	//	if (button == &openButton)  openButtonClicked();
	}

private:

	//void openButtonClicked()
	//{
	//	FileChooser chooser("Select a Wave file to play...",
	//		File::nonexistent,
	//		"*.wav");

	//	if (chooser.browseForFileToOpen())
	//	{
	//		File file(chooser.getResult());
	//		AudioFormatReader* reader = formatManager.createReaderFor(file);

	//		if (reader != nullptr)
	//		{
	//			ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource(reader, true);
	//			audioTransportSource.setSource(newSource, 0, nullptr, reader->sampleRate);
	//			//playButton.setEnabled(true);
	//			audioReaderSource = newSource.release();
	//		}
	//	}
	//}

	TextButton openButton;


	// Audio File Reading Variables
	AudioFormatManager formatManager;
	ScopedPointer<AudioFormatReaderSource> audioReaderSource;
	juce::AudioTransportSource audioTransportSource;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Som)
};



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;

private:

	ScopedPointer<RxComponent> rc = nullptr;

	ScopedPointer<Som> som = nullptr;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
