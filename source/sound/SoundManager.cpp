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
    FMOD_RESULT _result = _system->close();
    ERRCHECK(_result);
    _result = _system->release();
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

    FMOD_RESULT _result = _system->playSound(FMOD_CHANNEL_FREE, m_sounds[soundID], paused, &_channel);
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
    FMOD_RESULT _result = _system->set3DListenerAttributes(0, &toFmodVec(pos), &toFmodVec(velocity), &toFmodVec(forward), &toFmodVec(up));
    ERRCHECK(_result);
}

void SoundManager::updateSoundSystem()
{
    getInstance()._system->update();
}

void SoundManager::init()
{
    FMOD_RESULT _result = FMOD::System_Create(&_system);
    ERRCHECK(_result);

    _result = _system->getVersion(&_version);
    ERRCHECK(_result);

    if (_version < FMOD_VERSION) {
        std::cout << "You are using an unsupported version of FMOD" << std::endl;
        exit(-1);
    }

    _result = _system->getNumDrivers(reinterpret_cast<int*>(&_numdrivers));
    ERRCHECK(_result);

    if (_numdrivers == 0) {
        _result = _system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        ERRCHECK(_result);
    }
    else {
        _result = _system->getDriverCaps(0, &_caps, 0, &_speakermode); std::cout << _speakermode << std::endl;
        ERRCHECK(_result);

        // set the user selected speaker mode.
        _result = _system->setSpeakerMode(_speakermode);
        ERRCHECK(_result);

        // if the user has the 'Acceleration' slider set to off!  (bad for latency)
        if (_caps & FMOD_CAPS_HARDWARE_EMULATED) {
            // maybe print pout a warning?
            _result = _system->setDSPBufferSize(1024, 10);
            ERRCHECK(_result);
        }

        _result = _system->getDriverInfo(0, _name, 256, 0);
        ERRCHECK(_result);

        // Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it.
        if (strstr(_name, "SigmaTel")) {
            _result = _system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
            ERRCHECK(_result);
        }
    }

    // init _system with maximum of 100 channels
    _result = _system->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
    // if the speaker mode selected isn't supported by this soundcard, switch it back to stereo
    if (_result == FMOD_ERR_OUTPUT_CREATEBUFFER) {
        _result = _system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        ERRCHECK(_result);

        _result = _system->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);/* ... and re-init. */
        ERRCHECK(_result);
    }

    _result = _system->set3DSettings(1.0, _distanceFactor, 1.0f);
    ERRCHECK(_result);
}

void SoundManager::createAllSounds()
{
    for (SoundInfo soundInfo : s_soundInfos)
        m_sounds.push_back(createSound(soundInfo.fileName, soundInfo.is3D, soundInfo.isLoop));
}

FMOD::Sound * SoundManager::createSound(std::string filename, bool is3D, bool isLoop)
{
    std::string soundFilePath = filename;
    FMOD::Sound *_sound;
    FMOD::Channel *_channel = 0;
    FMOD_MODE _mode;

    if (is3D)
        _mode = FMOD_3D;
    else
        _mode = FMOD_SOFTWARE | FMOD_2D;

    FMOD_RESULT _result = _system->createSound(soundFilePath.c_str(), _mode, 0, &_sound);
    ERRCHECK(_result);

    if (is3D) {
        _result = _sound->set3DMinMaxDistance(0.5f * _distanceFactor, 5000.0f * _distanceFactor);
        ERRCHECK(_result);
    }

    if (isLoop) {
        _result = _sound->setMode(FMOD_LOOP_NORMAL);
        ERRCHECK(_result);
    }

    return _sound;
}
