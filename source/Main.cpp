
#include "fmod.hpp"

#include "Application.h"

void playStartUpSound()
{
    // Quick and dirty test
    
    FMOD::System * system;
    FMOD::Sound * sound;
    FMOD::Channel * channel;
    FMOD::System_Create(&system);
    system->init(100, FMOD_INIT_NORMAL, 0);
    system->createSound("data/stereo.ogg", FMOD_DEFAULT, 0, &sound);
    system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
}

int main(int argc, char* argv[])
{
    int result = -1;
    
    Application * app = new Application(argc, argv);

    playStartUpSound();
    
    result = app->exec();
    
    return result;
}
