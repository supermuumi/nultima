#pragma once

#include <map>
#include <string>

namespace Nultima
{

class AudioManager
{
public:
    AudioManager();
    ~AudioManager();

    void loadJSON(const char* fname);

    void addMusic(std::string id, const char* fname);
    void addEffect(std::string id, const char* fname, bool loop);

    void playMusic(std::string id);
    void playEffect(std::string id, bool restart);
    void stopMusic(std::string id);
    void stopEffect(std::string id);

private:
    std::map<std::string, unsigned int> m_musicTracks;
    std::map<std::string, unsigned int> m_effects;
};

};
