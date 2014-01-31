#pragma once

#include <glm/glm.hpp>
#include <fmod.hpp>

class Sound
{
public:
    enum SoundFile {diesel, alerts};

    Sound(SoundFile file, bool paused = false);
    Sound(SoundFile file, const glm::vec3 & pos, const glm::vec3 & velocity, bool paused = false);
    ~Sound();

    void setPaused(bool spaused);
    void setPosition(const glm::vec3 & pos);
    void setVelocity(const glm::vec3 & velocity);
    void setVolume(float volume);
    void setMute(bool mute);

private:
    FMOD::Channel * m_channel;
    bool m_is3D;
};