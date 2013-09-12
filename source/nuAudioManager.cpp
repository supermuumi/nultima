#include <iostream>
#include <map>
#include <string>

#include "nuAudioManager.h"
#include "nuFileUtils.h"

#include "bass/bass.h"
#include "rapidjson/document.h"

using namespace Nultima;

AudioManager::AudioManager()
{
    BASS_Init(-1,44100,0,0,NULL);
}

AudioManager::~AudioManager()
{
    BASS_Free();
}

void AudioManager::loadJSON(const char* fname)
{
    rapidjson::Document doc = FileUtils::readJSON(fname);

    const rapidjson::Value& music = doc["music"];
    for (rapidjson::Value::ConstMemberIterator itr = music.MemberBegin(); itr != music.MemberEnd(); ++itr)
    {
        std::string id = itr->name.GetString();
        std::string fname = itr->value.GetString();

        addMusic(id, fname.c_str());
    }

    const rapidjson::Value& effects = doc["effects"];
    for (rapidjson::Value::ConstMemberIterator itr = effects.MemberBegin(); itr != effects.MemberEnd(); ++itr)
    {
        std::string id = itr->name.GetString();
        const rapidjson::Value& arr = itr->value;
        std::string fname = arr[0u].GetString();
        bool loop = arr[1].GetBool();
        addEffect(id, fname.c_str(), loop);
    }
}

void AudioManager::addMusic(std::string id, const char* fname) 
{
    //std::cout << "AudioManager::addMusic(" << id << ", \"" << fname << "\")\n";
    m_musicTracks[id] = BASS_StreamCreateFile(FALSE, fname, 0, 0, BASS_SAMPLE_LOOP);
}

void AudioManager::addEffect(std::string id, const char* fname, bool loop) 
{
    //std::cout << "AudioManager::addEffect(" << id << ", \"" << fname << "\", "<<loop<<")\n";
    m_effects[id] = BASS_StreamCreateFile(FALSE, fname, 0, 0, loop?BASS_SAMPLE_LOOP:0);
}

void AudioManager::playMusic(std::string id) 
{
    BASS_ChannelPlay(m_musicTracks[id], FALSE);
}

void AudioManager::playEffect(std::string id, bool restart) 
{
    BASS_ChannelPlay(m_effects[id], restart);
}

void AudioManager::stopMusic(std::string id) 
{
    BASS_ChannelStop(m_effects[id]);
}

void AudioManager::stopEffect(std::string id) 
{
    BASS_ChannelStop(m_effects[id]);
}

