#include "nuAudioManager.h"
#include "bass/bass.h"

#include <map>
#include <string>

using namespace Nultima;

AudioManager::AudioManager()
{
    BASS_Init(-1,44100,0,0,NULL);
}

AudioManager::~AudioManager()
{
    BASS_Free();
}

void AudioManager::addMusic(std::string id, const char* fname) 
{
    m_musicTracks[id] = BASS_StreamCreateFile(FALSE, fname, 0, 0, BASS_SAMPLE_LOOP);
}

void AudioManager::addEffect(std::string id, const char* fname) 
{
    m_effects[id] = BASS_StreamCreateFile(FALSE, fname, 0, 0, 0);
}

void AudioManager::playMusic(std::string id) 
{
    BASS_ChannelPlay(m_musicTracks[id], FALSE);
}

void AudioManager::playEffect(std::string id) 
{
    BASS_ChannelPlay(m_effects[id], TRUE);
}

void AudioManager::stopMusic(std::string id) 
{
    BASS_ChannelStop(m_effects[id]);
}

void AudioManager::stopEffect(std::string id) 
{
    BASS_ChannelStop(m_effects[id]);
}

