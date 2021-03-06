#pragma once

#include <glm/glm.hpp>
#include <fmod.hpp>

class Sound
{
public:
    enum SoundFile { kButtonClick, kButtonSwitch, kImpact, kTypewriter, kLoop, kDeepNoise, kBeam };

    Sound(SoundFile file, bool paused = false);
    Sound(SoundFile file, const glm::vec3 & pos, const glm::vec3 & velocity, bool paused = false);
    ~Sound();

    void setPaused(bool paused);
    void setPosition(const glm::vec3 & position);
    void setVelocity(const glm::vec3 & velocity);
    void setVolume(float volume);
    void setMute(bool mute);
    void stop();

protected:
    FMOD::Channel * m_channel;
    
};
