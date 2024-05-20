#include "AudioSystem.hpp"
#include <UCodeRunTime/StaticComponentsIndex.hpp>
#include "UCodeRunTime/Core/GameRunTime.hpp"

#include "soloud.h"
#include "soloud_wav.h"
AudioStart

AudioSystem::AudioSystem(Gamelibrary* lib) : System(lib)
{
	soloud = std::make_unique<SoLoud::Soloud>();
	soloud->init();
};

void AudioSystem::Play(AudioFile& audio, const AudioPlaySettings& settings)
{
	auto handle = (SoLoud::Wav*)audio.GetHandle();
	soloud->play(*handle);
}

AudioFile::~AudioFile()
{
	if (base)
	{
		auto ptr = (SoLoud::Wav*)base;
		delete ptr;
	}
}
void AudioFile::Load(const Path& path)
{
	if (base)
	{
		auto ptr = (SoLoud::Wav*)base;
		delete ptr;
	}
	base = new SoLoud::Wav();

	SoLoud::Wav* v = (SoLoud::Wav*)base;

	auto s = path.generic_string();
	v->load(s.c_str());
}

AudioSystem* AudioSystem::Get(Gamelibrary* lib)
{
	constexpr StaticBooksIndex_t KeyIdex = (StaticBooksIndex_t)StaticBooksIndex::AudioSystem;
	auto V = lib->Get_StaticComponent(KeyIdex);
	if (V) { return (AudioSystem*)V; }

	AudioSystem* r = new AudioSystem(lib);;

	lib->MoveSystem(r);
	lib->SetStaticComponent(KeyIdex, r);
	return r;

}
AudioEnd
