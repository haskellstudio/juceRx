#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

namespace PSMixer
{
	const static int read_ahead_buffer_size =  16*1024;
	const static double stem_sample_rate = 44100.0;
	const static int stem_max_channels = 2;

	class DeviceManager
	{
		AudioFormatManager	formatManager;
		AudioSourcePlayer	sourcePlayer;
		AudioDeviceManager	deviceManager;
	public:
		DeviceManager()
		{
			formatManager.registerBasicFormats();
			deviceManager.initialiseWithDefaultDevices(0, 2);
		}

		~DeviceManager()
		{
			deviceManager.closeAudioDevice();
		}

		void setMixer(juce::AudioSource *mixerSource)
		{
			sourcePlayer.setSource(mixerSource);
			deviceManager.addAudioCallback(&sourcePlayer);
		}

		void resetMixer()
		{
			sourcePlayer.setSource(nullptr);
			deviceManager.removeAudioCallback(&sourcePlayer);
		}

		AudioFormatReader* getAudioFormatReader(String file_name)
		{
			if (File(file_name).existsAsFile())
			{
				return formatManager.createReaderFor(File(file_name));
			}
			else
			{
				return nullptr;
			}
		}

		AudioFormatReader* getAudioFormatReader(File file_name)
		{
			if (file_name.existsAsFile())
			{
				return formatManager.createReaderFor(file_name);
			}
			else
			{
				return nullptr;
			}
		}
	};

	class PSAudioSource : public AudioTransportSource
	{
		AudioFormatReader		*fileReader;
		AudioFormatReaderSource *sourceReader;
		juce::TimeSliceThread	readerThread;

	public:
		PSAudioSource() : sourceReader(nullptr), readerThread("Stem Reader Thread")
		{
			readerThread.startThread(5);
		}

		~PSAudioSource()
		{
			//readerThread.signalThreadShouldExit();
			//readerThread.waitForThreadToExit(1000);
			//stopThread();
			//if(sourceReader) delete sourceReader;
		}

		void setAudioFormatReader(AudioFormatReader * audio_reader)
		{
			fileReader = audio_reader;
			sourceReader = new AudioFormatReaderSource(fileReader, true);
			//setSource(sourceReader, read_ahead_buffer_size, &readerThread, fileReader->sampleRate, stem_max_channels);
			setSource(sourceReader, 0, nullptr, fileReader->sampleRate, stem_max_channels);
			sourceReader->setLooping(false);
		}

		//void setAudioFormatReader2(AudioFormatReader * audio_reader)
		//{
		//	fileReader = audio_reader;
		//	sourceReader = new AudioFormatReaderSource(fileReader, true);
		//	setSource(sourceReader, read_ahead_buffer_size, &readerThread, fileReader->sampleRate, stem_max_channels);
		//	//setSource(sourceReader, 0, nullptr, fileReader->sampleRate, stem_max_channels);
		//	sourceReader->setLooping(true);
		//}
	};

	class Mixer : public MixerAudioSource
	{
	public:
		void addStem(PSAudioSource *stem)
		{
			addInputSource(stem, true);
		}
	};
}





