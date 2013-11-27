
#include "fmod.hpp"

#include "Game.h"

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
    
//    playStartUpSound();
    
    Game * game = new Game(argc, argv);
    
    result = game->exec();
    
    return result;
}
