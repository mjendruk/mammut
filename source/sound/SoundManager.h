#pragma once

#include <QVector>
#include <QString>

#include <glm/glm.hpp>

#include "fmod.hpp"

//based on https://github.com/lanice/elemate
//implements the singleton pattern, see http://stackoverflow.com/questions/1008019/c-singleton-design-pattern

class SoundManager
{
    friend class Sound;
public:
    static SoundManager & instance();

    static void checkError(FMOD_RESULT);
    static FMOD_VECTOR toFmodVec(const glm::vec3 & glmvec);

    FMOD::Channel * createNewChannel2D(int soundID, bool paused);
    FMOD::Channel * createNewChannel3D(int soundID, bool paused, FMOD_VECTOR pos, FMOD_VECTOR vel);

    void setListenerAttributes(const glm::vec3 & position, const glm::vec3 & forward, const glm::vec3 & up, const glm::vec3 & velocity);
    void updateSoundSystem();

protected:

    typedef struct SoundInfo {
        QString fileName;
        bool is3D;
        bool isLoop;
    };

    static const QVector<SoundInfo> s_soundInfos;

    SoundManager();
    ~SoundManager();

    SoundManager(SoundManager const&) = delete;
    void operator=(SoundManager const&) = delete;

    void init();
    void createAllSounds();
    FMOD::Sound * createSound(QString filename, bool is3D, bool isLoop);

protected:
    float m_distanceFactor = 10.0f;
    QVector<FMOD::Sound*> m_sounds;
    FMOD::System * m_system;
};
