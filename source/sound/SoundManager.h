#pragma once

#include <vector>
#include <string>

#include <QObject>

#include <glm/glm.hpp>

#include "fmod.hpp"

//based on https://github.com/lanice/elemate
//implements the singleton pattern, see http://stackoverflow.com/questions/1008019/c-singleton-design-pattern

class SoundManager : public QObject
{
    Q_OBJECT

public:
    static SoundManager & getInstance()
    {
        static SoundManager instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    static void ERRCHECK(FMOD_RESULT);
    static FMOD_VECTOR toFmodVec(glm::vec3 glmvec);

    FMOD::Channel * createNewChannel2D(int soundID, bool paused);
    FMOD::Channel * createNewChannel3D(int soundID, bool paused, FMOD_VECTOR pos, FMOD_VECTOR vel);

    static void setPaused(FMOD::Channel * channel, bool paused);
    static void setSoundPos(FMOD::Channel * channel, FMOD_VECTOR pos);
    static void setSoundVel(FMOD::Channel * channel, FMOD_VECTOR vel);
    static void setMute(FMOD::Channel * channel, bool mute);
    static void setVolume(FMOD::Channel * channel, float vol);

    void setListenerAttributes(const glm::vec3 & pos, const glm::vec3 & forward, const glm::vec3 & up, const glm::vec3 & velocity);
    void updateSoundSystem();

protected:

    typedef struct SoundInfo {
        std::string fileName;
        bool is3D;
        bool isLoop;
    } SoundInfo;

    static const std::vector<SoundInfo> s_soundInfos;

    SoundManager();
    ~SoundManager();

    SoundManager(SoundManager const&);
    void operator=(SoundManager const&);

    void init();
    void createAllSounds();
    FMOD::Sound * createSound(std::string filename, bool is3D, bool isLoop);

protected:
    float m_distanceFactor = 10.0f;
    std::vector<FMOD::Sound*> m_sounds;
    FMOD::System * m_system;
};
