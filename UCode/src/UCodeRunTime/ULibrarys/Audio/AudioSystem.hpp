#include "AudioNamespace.hpp"
#include "UCodeRunTime/Core/System.hpp"
#include "UCodeRunTime/BasicTypes.hpp"

namespace SoLoud
{
	class Soloud;
};

AudioStart

struct AudioFile
{
private:
  void* base;
public:
	AudioFile()
	{

	}
	~AudioFile();

	void Load(const Path& path);

	void* GetHandle()
	{
		return base;
	}
};
struct AudioPlaySettings
{

};
class AudioSystem :private System
{
private:
	AudioSystem(Gamelibrary* lib);
	Unique_ptr<SoLoud::Soloud> soloud;
public:

	static AudioSystem* Get(Gamelibrary* lib);
	void Play(AudioFile& audio,const AudioPlaySettings& settings);
public:
	
};

AudioEnd