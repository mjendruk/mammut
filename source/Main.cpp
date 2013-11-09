
#include "fmod.hpp"

#include "Canvas.h"
#include "Application.h"
#include "Painter.h"

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
    
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    Canvas * canvas = new Canvas(format);
    canvas->setContinuousRepaint(true, 0);
    canvas->setSwapInterval(Canvas::VerticalSyncronization);

    Painter painter;
    canvas->assignPainter(&painter);
    canvas->show();

    result = app->exec();

    delete app;
    delete canvas;

    return result;
}
