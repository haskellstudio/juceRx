#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

#include <sstream>
#include <thread>
#include <string>
using namespace std;


std::string get_pid();

void msg(juce::String content, juce::String title = "", juce::String buttontext = "");

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

	class PSAudioSource : public AudioTransportSource // , public ChangeListener
	{
		AudioFormatReader		*fileReader;
		AudioFormatReaderSource *sourceReader;
		juce::TimeSliceThread	readerThread;
	public:
		float _startTime = -1.0f;
		float _endTime = -1.0f;

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

		//void changeListenerCallback(ChangeBroadcaster* source) override
		//{
		//	if (source == this)
		//	{

		//	}
		//}


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


		void addStem(PSAudioSource *stem, int type, bool isMain = false)
		{
			if (isMain)
				mainAS = stem;
			else
			{
				//need improve
				if (type == 0)
				{
					type0AudioSrc = stem;
				}
				else if (type == 1)
				{
					type1AudioSrc = stem;
				}
			}

			addInputSource(stem, true);
		}

		void getNextAudioBlock(const AudioSourceChannelInfo& info) override
		{
			MixerAudioSource::getNextAudioBlock(info);

			if (mainAS)
				g_pos = mainAS->getCurrentPosition();

			//need improve
			if (type0AudioSrc)
			{
				float pos = type0AudioSrc->getCurrentPosition();
				if (pos  >= type0AudioSrc->_endTime)
				{
					type0AudioSrc->setGain(0.0);
				}
			}

			if (type1AudioSrc)
			{
				float pos = type1AudioSrc->getCurrentPosition();
				if (pos >= type1AudioSrc->_endTime)
				{
					type1AudioSrc->setGain(0.0);
				}
			}

			//std::string newpid = get_pid();
			//if (oldpid != newpid)
			//{
			//	oldpid = newpid;
			//	msg(newpid);
			//}
			
		}

		PSAudioSource * mainAS = nullptr;


		//need improve
		PSAudioSource * type0AudioSrc = nullptr;
		PSAudioSource * type1AudioSrc = nullptr;
	};
}





struct units
{
	int type = 1000;
	float typetime = -1000.0;
	float timeDifference = 200.0;
	units( int t, float tt, float td)
	{
		type = t;
		typetime = tt;
		timeDifference = td;
	}

};


struct BeatType
{
	int type = 0;
	float typetime = 0.0;			// zhe ge yin chi xu de shi jian 
	float typetimeend = 0.0;
	
	//double typeerrortime = 0.0;	//  type1time zuo you fan wei duo shao nei , hai suan zhe ge yin .
	BeatType(int t, float tt, float ttend)
	{
		type = t;
		typetime = tt;
		typetimeend = ttend;
	}
	
};


struct Drum
{
	float _curTime = -1000.0;
	float _window = 1.0f;
	juce::Array<BeatType> songArray;

	int typeIndexs[128] = { 0 };

	int typeCount = 0;
	float aheadTime = 0;
	Drum::Drum()
	{
	
		playStart(1);
		
	}

	ScopedPointer<PSMixer::DeviceManager>  deviceManager;
	PSMixer::PSAudioSource*  audio_source_01 = nullptr;

	// need improve
	PSMixer::PSAudioSource*  audio_source_type_0 = nullptr;
	PSMixer::PSAudioSource*  audio_source_type_1 = nullptr;


	void startPlayOrginalSong()
	{
		audio_source_01->start();
	}

	void playDrumForAPeriod()
	{
		//audio_source_type_1->addChangeListener
	}
	void loadRes(int i)
	{
		deviceManager = nullptr;
		if (i == 1)
		{
			deviceManager = new PSMixer::DeviceManager;
			File file_01("C:/Users/zhufei/Desktop/Dennis Kuo - Sweet Saturn.mp3");
			//File file_02("C:/Users/zhufei/Desktop/mp3/dusk.mp3");
			File file_02("C:/Users/zhufei/Desktop/Dennis Kuo - Sweet Saturn_type12.mp3");
			File file_03("C:/Users/zhufei/Desktop/mp3/killer.mp3");
			audio_source_01 = new PSMixer::PSAudioSource;
			audio_source_01->setAudioFormatReader(deviceManager->getAudioFormatReader(file_01));
			//audio_source_01->addChangeListener(audio_source_01);

			//need improve
			audio_source_type_0 = new PSMixer::PSAudioSource;
			audio_source_type_0->setAudioFormatReader(deviceManager->getAudioFormatReader(file_02));
			//audio_source_type_0->addChangeListener(audio_source_type_0);

			audio_source_type_1 = new PSMixer::PSAudioSource;
			audio_source_type_1->setAudioFormatReader(deviceManager->getAudioFormatReader(file_02));

		//	audio_source_03 = new PSMixer::PSAudioSource;
		//	audio_source_03->setAudioFormatReader(deviceManager.getAudioFormatReader(file_03));/**/

			PSMixer::Mixer *mixer = new PSMixer::Mixer;
			mixer->addStem(audio_source_01, -1, true);

			//need improve
			mixer->addStem(audio_source_type_0, 0);
			mixer->addStem(audio_source_type_1, 1);

		//	mixer->addStem(audio_source_03);


			deviceManager->setMixer(mixer);
			

			audio_source_01->setGain(0.2);
			audio_source_type_0->setGain(1.0);
			audio_source_type_1->setGain(1.0);
		//	audio_source_03->setGain(1.0);

			//audio_source_01->start();
			//audio_source_02->start();
			//audio_source_03->start();

			//AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "getchar", "getchar", "getchar");

		}
	}

	void getsong(int i)
	{
		aheadTime = -1;
		typeCount = 2;
		if (i == 1)
		{
			BeatType song1types[] = {
				{0, 0.001f, 	0.22f},
				{ 1, 0.6f, 	1.0f}, //0.75f },
				{ 0, 1.10f,	1.270f },
				{ 0, 1.270f, 	1.460f },
				{ 1, 1.890f,      2.050f }

			};

			//BeatType song1types[] = {
			//	{ 0, 0.001f, 	0.22f },
			//	{ 1, 0.2f, 	0.22f },
			//	{ 0, 0.41f, 	0.22f },
			//	{ 1, 0.5f, 	0.22f },
			//	{ 0, 0.7f, 	0.22f },
			//	{ 0, 0.9f, 	0.22f },
			//	{ 0, 1.10f,	1.270f },
			//	{ 0, 1.270f, 	1.460f },
			//	{ 1, 1.890f,      2.050f }

			//};


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

	void playStart(int arg)
	{
		loadRes(arg);
		getsong(arg);
		for (int i = 0; i < typeCount; i++)
		{
			typeIndexs[i] = 0;
				for (int k = 0; k < songArray.size(); k++)
				{
					if (songArray.getReference(k).type == i)
					{
						typeIndexs[i] = k;

						// need improve
						if (i == 0)
						{
							if (audio_source_type_0)
							{
								audio_source_type_0->_startTime = songArray.getReference(k).typetime;
								audio_source_type_0->_endTime = songArray.getReference(k).typetimeend;
							}
						}
						else if (i == 1)
						{
							audio_source_type_1->_startTime = songArray.getReference(k).typetime;
							audio_source_type_1->_endTime = songArray.getReference(k).typetimeend;
						}
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



	void ajust(float curTime)
	{
		_curTime = curTime;
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


									float st = songArray.getReference(k).typetime;
									float et = songArray.getReference(k).typetimeend;
									// need improve
									if (i == 0)
									{
										audio_source_type_0->setPosition(st);
										audio_source_type_0->_startTime = st;
										audio_source_type_0->_endTime = et;
									}
									else if(i == 1)
									{
										audio_source_type_1->setPosition(st);
										audio_source_type_1->_startTime = st;
										audio_source_type_1->_endTime = et;
									}
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


	void getUnitToDraw(juce::Array<units> & arg)
	{
		float shortestTime = 0.0f;
		int shortestIndex = 0;
		for (int itype = 0; itype < typeCount; itype++)
		{
			int index = getIndex(itype);
			float time = getCurTypeTimeByIndex(index);
			if (time < shortestTime)
			{
				shortestTime = time;
				shortestIndex = index;
			}
		}
		
		for (int i = shortestIndex; i < songArray.size(); i++)
		{
			if (songArray.getReference(i).typetime - _curTime <= _window)
			{
				units u(songArray.getReference(i).type, songArray.getReference(i).typetime, (songArray.getReference(i).typetime -  _curTime)/_window  );
				arg.add(u);
			}
			else
			{
				break;
			}
		}
		if (arg.size() > 1)
		{
			shortestIndex = 0;
		}

	}

};






//juce::Array<unit> * songArray = new juce::Array<unit>;




struct LockFreeArray
{

	static const int _row = 512;

	static const int _idleCount = 50;

	int _wi = 0; //write index
	int _ri = 0; //read index

	bool _forceWrite = true;
	juce::Array<units> allComing[_row];

	int incRead()
	{
		if (_ri == _row - 1)
		{
			_ri = 0;
		}
		else
		{
			_ri++;
		}
	}

	int incWrite()
	{
		if (_wi == _row - 1)
		{
			_wi = 0;
		}
		else
		{
			_wi++;
		}
	}

	bool write(juce::Array<units> &  arg)
	{
		if (_wi < _ri)
		{
			if (_ri  - _wi <= _idleCount)
			{
				DBG("lock free Array is full, cannot wirte");
				return false;
			}
		}
		else // _wi >= _ri
		{
			int idle = _row - _wi + _ri  ;
			if (idle <= _idleCount)
			{
				DBG("lock free Array is full, cannot wirte");
				return false;
			}
		}
		DBG("Write index: " << _wi << " Read index: " << _ri);

		int oldw = _wi;
	
		allComing[oldw] = arg;
		incWrite();
		return true;
	}

	bool read(juce::Array<units> & arg)
	{
		if (_ri != _wi )
		{
			int oldr = _ri;
			incRead();
			arg = allComing[oldr];
			allComing[oldr].clear();
			return true;
		}
		else
		{
			return false;
		}
	}




	
	//juce::Array<   >  pAllArrays[100];
};





//AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, juce::String(i), "oopguard drum1 error", "oopguard drum1 error");

