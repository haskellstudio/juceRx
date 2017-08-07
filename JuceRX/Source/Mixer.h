#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

#include <sstream>
#include <thread>
#include <string>
using namespace std;


std::string get_pid();

void showmsg(juce::String content, juce::String title = "", juce::String buttontext = "");

extern std::string oldpid;
extern double g_pos;
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
		void addStem(PSAudioSource *stem, bool isMain = false)
		{
			if (isMain)
				mainAS = stem;
			addInputSource(stem, true);
		}

		void getNextAudioBlock(const AudioSourceChannelInfo& info) override
		{
			MixerAudioSource::getNextAudioBlock(info);

			if (mainAS)
				g_pos = mainAS->getCurrentPosition();

			//std::string newpid = get_pid();
			//if (oldpid != newpid)
			//{
			//	oldpid = newpid;
			//	showmsg(newpid);
			//}
			
		}

		PSAudioSource * mainAS = nullptr;
	};
}




struct BeatType
{
	int type = 0;
	double typetime = 0.0;			// zhe ge yin chi xu de shi jian 
	double typetimeend = 0.0;
	//double typeerrortime = 0.0;	//  type1time zuo you fan wei duo shao nei , hai suan zhe ge yin .
	BeatType(int t, double tt, double ttend)
	{
		type = t;
		typetime = tt;
		typetimeend = ttend;
	}
	
};


struct Drum
{
	double _pos;

	juce::Array<BeatType> songArray;

	int typeIndexs[128] = { 0 };
	int typeCount = 2;
	float aheadTime = 0;
	Drum::Drum()
	{
		_pos = 0;
		playStart();
	}
	void getsong(int i)
	{
		aheadTime = -5;
		if (i == 1)
		{
			BeatType song1types[] = {
				{0, 0.001f, 	0.22f},
				{ 1, 0.6f, 	0.75f },
				{ 0, 1.10f,	1.270f },
				{ 0, 1.270f, 	1.460f },
				{ 1, 1.890f,      2.050f }

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

	void playStart()
	{
		getsong(1);
		for (int i = 0; i < typeCount; i++)
		{
			typeIndexs[i] = 0;
				for (int k = 0; k < songArray.size(); k++)
				{
					if (songArray.getReference(k).type == i)
					{
						typeIndexs[i] = k;
						break;
					}
				}
		}

	}

	double getCurTypeTimeByIndex(int index)
	{
		return songArray.getReference(index).typetime;
	}

	double getCurTypeTimeEndByIndex(int index)
	{
		return songArray.getReference(index).typetimeend;
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
									typeIndexs[i] = k;
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

