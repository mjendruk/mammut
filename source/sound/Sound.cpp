#include "Sound.h"

#include "SoundManager.h"


Sound::Sound(SoundFile file, bool paused)
{
    m_channel = SoundManager::instance().createNewChannel2D(file, paused);
}

Sound::Sound(SoundFile file, const glm::vec3 & pos, const glm::vec3 & velocity, bool paused)
{
    m_channel = SoundManager::instance().createNewChannel3D(file, paused, SoundManager::toFmodVec(pos), SoundManager::toFmodVec(velocity));
}

Sound::~Sound()
{

}

void Sound::setPaused(bool paused)
{
    FMOD_RESULT result = m_channel->setPaused(paused);
    SoundManager::checkError(result);
}

void Sound::setPosition(const glm::vec3 & position)
{
    FMOD_VECTOR oldVelocity;
    FMOD_VECTOR oldPosition;
    m_channel->get3DAttributes(&oldPosition, &oldVelocity);

    FMOD_VECTOR newPosition = SoundManager::toFmodVec(position);
    FMOD_RESULT result = m_channel->set3DAttributes(&newPosition, &oldVelocity);

    SoundManager::checkError(result);
}

void Sound::setVelocity(const glm::vec3 & velocity)
{
    FMOD_VECTOR oldVelocity;
    FMOD_VECTOR oldPosition;
    m_channel->get3DAttributes(&oldPosition, &oldVelocity);

    FMOD_VECTOR newVelocity = SoundManager::toFmodVec(velocity);
    FMOD_RESULT result = m_channel->set3DAttributes(&oldPosition, &newVelocity);

    SoundManager::checkError(result);
}

void Sound::setVolume(float volume)
{
    FMOD_RESULT result = m_channel->setVolume(volume);
    SoundManager::checkError(result);
}

void Sound::setMute(bool mute)
{
    FMOD_RESULT result = m_channel->setMute(mute);
    SoundManager::checkError(result);
}
