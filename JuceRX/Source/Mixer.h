#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

#include <sstream>
#include <thread>
#include <string>
using namespace std;


std::string get_pid();

void showmsg(juce::String content, juce::String title = "", juce::String buttontext = "");

extern std::string oldpid;

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


	class Mixer : public juce::MixerAudioSource
	{
	public:
		void addStem(PSAudioSource *stem, bool isMain = true)
		{
			if (isMain)
				mainAS = stem;
			addInputSource(stem, true);
		}

		void getNextAudioBlock(const AudioSourceChannelInfo& info) override
		{
			MixerAudioSource::getNextAudioBlock(info);


			std::string newpid = get_pid();
			if (oldpid != newpid)
			{
				oldpid = newpid;
				showmsg(newpid);
			}
			
		}

		PSAudioSource * mainAS = nullptr;
	};
}




struct BeatType
{
	int type = 0;
	double typetime = 0.0;			// zhe ge yin chi xu de shi jian 
	//double typeerrortime = 0.0;	//  type1time zuo you fan wei duo shao nei , hai suan zhe ge yin .
	BeatType(int t, double tt)
	{
		type = t;
		typetime = tt;
	}
	
};


struct Drum
{

	juce::Array<BeatType> songArray;

	int typeIndexs[128] = { 0 };
	int typeCount = 2;
	Drum::Drum()
	{
		getsong(1);
	}
	void getsong(int i)
	{
		
		if (i == 1)
		{
			BeatType song1types[] = {
				{1, 1.0f },    
				{0, 2.0f}

			};
			songArray.addArray(song1types, sizeof(song1types) / sizeof(BeatType));
		}

	}

	int getIndex(int type)
	{
		if (type >= 0 && type < 128)
		{
			return typeIndexs[type];
		}
		else
		{
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, juce::String(type), "getIndex error", "ok");
			return 0;
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
						for (int k = j + 1; k < songArray.size(); k++)
						{
							if (songArray.getReference(k).type == i)
							{
								double threshHold = songArray.getReference(j).typetime + (
									(songArray.getReference(k).typetime - songArray.getReference(j).typetime) / 2
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
		}
	}

};












//AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, juce::String(i), "oopguard drum1 error", "oopguard drum1 error");

