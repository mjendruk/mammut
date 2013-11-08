
#include "Canvas.h"
#include "Application.h"
#include "Painter.h"

int main(int argc, char* argv[])
{
    int result = -1;

    Application * app = new Application(argc, argv);

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
