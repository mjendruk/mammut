#include "SoundManager.h"

#include <iostream>

#include "fmod_errors.h"



const std::vector<SoundManager::SoundInfo> SoundManager::s_soundInfos = {
    { "data/sounds/diesel.mp3", true, false },
    { "data/sounds/alerts.ogg" /*example*/, true, false } };


SoundManager::SoundManager()
{
    init();
    createAllSounds();
}

SoundManager::~SoundManager()
{
    for (FMOD::Sound * sound : m_sounds)
        sound->release();
    FMOD_RESULT _result = m_system->close();
    ERRCHECK(_result);
    _result = m_system->release();
    ERRCHECK(_result);
}

void SoundManager::ERRCHECK(FMOD_RESULT _result)
{
    if (_result != FMOD_OK)
        std::cout << "FMOD error! "<< FMOD_ErrorString(_result) << " (" << _result << ")" << std::endl;
}

FMOD_VECTOR SoundManager::toFmodVec(glm::vec3 glmvec)
{
    return { glmvec.x, glmvec.y, glmvec.z };
}

FMOD::Channel * SoundManager::createNewChannel2D(int soundID, bool paused)
{
    FMOD::Channel *_channel = nullptr;

    FMOD_RESULT _result = m_system->playSound(FMOD_CHANNEL_FREE, m_sounds[soundID], paused, &_channel);
    ERRCHECK(_result);

    return _channel;
}

FMOD::Channel * SoundManager::createNewChannel3D(int soundID, bool paused, FMOD_VECTOR pos, FMOD_VECTOR vel)
{
    FMOD::Channel *_channel = createNewChannel2D(soundID, paused);

    FMOD_RESULT _result = _channel->set3DAttributes(&pos, &vel);
    ERRCHECK(_result);

    return _channel;
}

void SoundManager::setPaused(FMOD::Channel * channel, bool paused)
{
    FMOD_RESULT _result = channel->setPaused(paused);
    ERRCHECK(_result);
}

void SoundManager::setSoundPos(FMOD::Channel * channel, FMOD_VECTOR pos)
{
    FMOD_VECTOR _vel;
    FMOD_VECTOR _pos;
    channel->get3DAttributes(&_pos, &_vel);

    FMOD_RESULT _result = channel->set3DAttributes(&pos, &_vel);

    ERRCHECK(_result);
}

void SoundManager::setSoundVel(FMOD::Channel * channel, FMOD_VECTOR vel)
{
    FMOD_VECTOR _vel;
    FMOD_VECTOR _pos;
    channel->get3DAttributes(&_pos, &_vel);

    FMOD_RESULT _result = channel->set3DAttributes(&_pos, &vel);
    ERRCHECK(_result);
}

void SoundManager::setMute(FMOD::Channel * channel, bool mute)
{
    FMOD_RESULT _result = channel->setMute(mute);
    ERRCHECK(_result);
}


void SoundManager::setVolume(FMOD::Channel * channel, float vol)
{
    FMOD_RESULT _result = channel->setVolume(vol);
    ERRCHECK(_result);
}

void SoundManager::setListenerAttributes(const glm::vec3 & pos, const glm::vec3 & forward, const glm::vec3 & up, const glm::vec3 & velocity)
{
    FMOD_RESULT _result = m_system->set3DListenerAttributes(0, &toFmodVec(pos), &toFmodVec(velocity), &toFmodVec(forward), &toFmodVec(up));
    ERRCHECK(_result);
}

void SoundManager::updateSoundSystem()
{
    m_system->update();
}

void SoundManager::init()
{
    FMOD_RESULT _result = FMOD::System_Create(&m_system);
    ERRCHECK(_result);

    unsigned int version = 0;
    _result = m_system->getVersion(&version);
    ERRCHECK(_result);

    if (version < FMOD_VERSION) {
        std::cout << "Error: You are using an unsupported version of FMOD" << std::endl;
        exit(-1);
    }

    int numdrivers = -1;
    _result = m_system->getNumDrivers(&numdrivers);
    ERRCHECK(_result);

    if (numdrivers == 0) {
        _result = m_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        ERRCHECK(_result);
    }
    else {
        FMOD_CAPS caps;
        FMOD_SPEAKERMODE speakermode;
        _result = m_system->getDriverCaps(0, &caps, 0, &speakermode);
        ERRCHECK(_result);

        // set the user selected speaker mode.
        _result = m_system->setSpeakerMode(speakermode);
        ERRCHECK(_result);

        // if the user has the 'Acceleration' slider set to off!  (bad for latency)
        if (caps & FMOD_CAPS_HARDWARE_EMULATED) {
            std::cout << "Warning: Sound is not hardware accelerated, it might lag behind" << std::endl;
            _result = m_system->setDSPBufferSize(1024, 10);
            ERRCHECK(_result);
        }

        char name[256];
        _result = m_system->getDriverInfo(0, name, 256, 0);
        ERRCHECK(_result);

        // Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it.
        if (strstr(name, "SigmaTel")) {
            _result = m_system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
            ERRCHECK(_result);
        }
    }

    // init _system with maximum of 100 channels
    _result = m_system->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
    // if the speaker mode selected isn't supported by this soundcard, switch it back to stereo
    if (_result == FMOD_ERR_OUTPUT_CREATEBUFFER) {
        _result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        ERRCHECK(_result);

        _result = m_system->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);/* ... and re-init. */
        ERRCHECK(_result);
    }

    _result = m_system->set3DSettings(1.0, m_distanceFactor, 1.0f);
    ERRCHECK(_result);

}

void SoundManager::createAllSounds()
{
    for (SoundInfo soundInfo : s_soundInfos)
        m_sounds.push_back(createSound(soundInfo.fileName, soundInfo.is3D, soundInfo.isLoop));
}

FMOD::Sound * SoundManager::createSound(std::string filename, bool is3D, bool isLoop)
{
    FMOD::Sound * sound;
    FMOD_MODE mode;

    if (is3D)
        mode = FMOD_3D;
    else
        mode = FMOD_SOFTWARE | FMOD_2D;

    FMOD_RESULT _result = m_system->createSound(filename.c_str(), mode, 0, &sound);
    ERRCHECK(_result);

    if (is3D) {
        _result = sound->set3DMinMaxDistance(0.5f * m_distanceFactor, 5000.0f * m_distanceFactor);
        ERRCHECK(_result);
    }

    if (isLoop) {
        _result = sound->setMode(FMOD_LOOP_NORMAL);
        ERRCHECK(_result);
    }

    return sound;
}
