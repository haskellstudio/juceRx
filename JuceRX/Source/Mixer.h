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




struct BeatType
{
	int type = 0;
	double typetime = 0.0;			// zhe ge yin chi xu de shi jian 
	//double typeerrortime = 0.0;	//  type1time zuo you fan wei duo shao nei , hai suan zhe ge yin .

	
};


struct Drum
{

	juce::Array<BeatType> songArray;

	int typeIndexs[128] = { 0 };
	int typeCount = 2;

	void getsong(int i)
	{
		
		if (i == 1)
		{
			BeatType song1types[] = {
				{1, 1.0f },    
				{2, 2.0f}

			};
			songArray.addArray(song1types, sizeof(song1types) / sizeof(BeatType));
		}

	}

	void playEnd()
	{
		for (int i = 0; i < 128; i++)
		{
			typeIndexs[i] = 0;
		}
	}

	void ajust(double curTime)
	{
		for (int i = 0; i < typeCount; i++)
		{
			int j = typeIndexs[i];  // dang qian sheng yin lei xing de suo yin
			for (; j < songArray.size(); j++)
			{
				if (songArray.getReference(j).type == i)
				{
					if (j <= songArray.size()-2)
					{
						int threshHold = songArray.getReference(j).typetime + (
							(songArray.getReference(j + 1).typetime - songArray.getReference(j).typetime) / 2
							);

						if (curTime >= threshHold)
						{
							typeIndexs[i]++;
						}
						else if (curTime < threshHold)
						{
							break;
						}

					}

				}
				

			}
		}
	}

};












//AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, juce::String(i), "oopguard drum1 error", "oopguard drum1 error");

