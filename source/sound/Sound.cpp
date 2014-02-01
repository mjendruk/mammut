#include "Sound.h"

#include "SoundManager.h"


Sound::Sound(SoundFile file, bool paused)
{
    m_channel = SoundManager::getInstance().createNewChannel2D(file, paused);
}

Sound::Sound(SoundFile file, const glm::vec3 & pos, const glm::vec3 & velocity, bool paused)
{
    m_channel = SoundManager::getInstance().createNewChannel3D(file, paused, SoundManager::toFmodVec(pos), SoundManager::toFmodVec(velocity));
}

Sound::~Sound()
{

}

void Sound::setPaused(bool paused)
{
    SoundManager::setPaused(m_channel, paused);
}

void Sound::setPosition(const glm::vec3 & position)
{
    SoundManager::setSoundPos(m_channel, SoundManager::toFmodVec(position));
}

void Sound::setVelocity(const glm::vec3 & velocity)
{
    SoundManager::setSoundVel(m_channel, SoundManager::toFmodVec(velocity));
}

void Sound::setVolume(float volume)
{
    SoundManager::setVolume(m_channel, volume);
}

void Sound::setMute(bool mute)
{
    SoundManager::setMute(m_channel, mute);
}
