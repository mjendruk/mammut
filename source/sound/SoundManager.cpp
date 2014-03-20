#include "SoundManager.h"

#include <QDebug>
#include <fmod_errors.h>

const QVector<SoundManager::SoundInfo> SoundManager::s_soundInfos = {
    { "data/sounds/button_click.wav", false, false },
    { "data/sounds/button_switch.wav", false, false },
    { "data/sounds/impact.wav", false, false },
    { "data/sounds/typewriter.wav", false, false },
    { "data/sounds/loop.wav", false, true },
    { "data/sounds/beam.wav", false, false } };


SoundManager::SoundManager()
{
    init();
    createAllSounds();
}

SoundManager::~SoundManager()
{
    for (FMOD::Sound * sound : m_sounds)
        sound->release();
    FMOD_RESULT result = m_system->close();
    checkError(result);
    result = m_system->release();
    checkError(result);
}

SoundManager & SoundManager::instance()
{
    static SoundManager instance;
    return instance;
}

void SoundManager::checkError(FMOD_RESULT result)
{
    if (result != FMOD_OK)
        qDebug() << "FMOD error! "<< FMOD_ErrorString(result) << " (" << result << ")";
}

FMOD_VECTOR SoundManager::toFmodVec(const glm::vec3 & glmvec)
{
    return { glmvec.x, glmvec.y, glmvec.z };
}

FMOD::Channel * SoundManager::createNewChannel2D(int soundID, bool paused)
{
    FMOD::Channel * channel = nullptr;

    FMOD_RESULT result = m_system->playSound(FMOD_CHANNEL_FREE, m_sounds[soundID], paused, &channel);
    checkError(result);

    return channel;
}

FMOD::Channel * SoundManager::createNewChannel3D(int soundID, bool paused, FMOD_VECTOR position, FMOD_VECTOR velocity)
{
    FMOD::Channel *channel = createNewChannel2D(soundID, paused);

    FMOD_RESULT result = channel->set3DAttributes(&position, &velocity);
    checkError(result);

    return channel;
}



void SoundManager::setListenerAttributes(const glm::vec3 & position, const glm::vec3 & forward, const glm::vec3 & up, const glm::vec3 & velocity)
{
    FMOD_VECTOR fmodPosition = toFmodVec(position);
    FMOD_VECTOR fmodForward = toFmodVec(forward);
    FMOD_VECTOR fmodUp = toFmodVec(up);
    FMOD_VECTOR fmodVelocity = toFmodVec(velocity);
    
    FMOD_RESULT result = m_system->set3DListenerAttributes(0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp);
    checkError(result);
}

void SoundManager::updateSoundSystem()
{
    FMOD_RESULT result = m_system->update();
    checkError(result);
}

void SoundManager::init()
{
    FMOD_RESULT result = FMOD::System_Create(&m_system);
    checkError(result);

    unsigned int version = 0;
    result = m_system->getVersion(&version);
    checkError(result);

    if (version < FMOD_VERSION) {
       qDebug() << "Error: You are using an unsupported version of FMOD";
        exit(-1);
    }

    int numdrivers = -1;
    result = m_system->getNumDrivers(&numdrivers);
    checkError(result);

    if (numdrivers == 0) {
        result = m_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        checkError(result);
    }
    else {
        FMOD_CAPS caps;
        FMOD_SPEAKERMODE speakermode;
        result = m_system->getDriverCaps(0, &caps, 0, &speakermode);
        checkError(result);

        // set the user selected speaker mode.
        result = m_system->setSpeakerMode(speakermode);
        checkError(result);

        // if the user has the 'Acceleration' slider set to off!  (bad for latency)
        if (caps & FMOD_CAPS_HARDWARE_EMULATED) {
            qDebug() << "Warning: Sound is not hardware accelerated, it might lag behind";
            result = m_system->setDSPBufferSize(1024, 10);
            checkError(result);
        }

        char name[256];
        result = m_system->getDriverInfo(0, name, 256, 0);
        checkError(result);

        // Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it.
        if (strstr(name, "SigmaTel")) {
            result = m_system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
            checkError(result);
        }
    }

    // init _system with maximum of 100 channels
    result = m_system->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
    // if the speaker mode selected isn't supported by this soundcard, switch it back to stereo
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER) {
        result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        checkError(result);

        result = m_system->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);/* ... and re-init. */
        checkError(result);
    }

    result = m_system->set3DSettings(1.0, m_distanceFactor, 1.0f);
    checkError(result);

}

void SoundManager::createAllSounds()
{
    for (SoundInfo soundInfo : s_soundInfos)
        m_sounds.push_back(createSound(soundInfo.fileName, soundInfo.is3D, soundInfo.isLoop));
}

FMOD::Sound * SoundManager::createSound(const QString & fileName, bool is3D, bool isLoop)
{
    FMOD::Sound * sound;
    FMOD_MODE mode;

    if (is3D)
        mode = FMOD_3D;
    else
        mode = FMOD_SOFTWARE | FMOD_2D;

    FMOD_RESULT result = m_system->createSound(fileName.toLatin1().data(), mode, 0, &sound);
    checkError(result);

    if (is3D) {
        result = sound->set3DMinMaxDistance(0.5f * m_distanceFactor, 5000.0f * m_distanceFactor);
        checkError(result);
    }

    if (isLoop) {
        result = sound->setMode(FMOD_LOOP_NORMAL);
        checkError(result);
    } else {
        result = sound->setMode(FMOD_LOOP_OFF);
        checkError(result);
    }

    return sound;
}
