#include "Application.h"

int main(int argc, char* argv[])
{
    int result = -1;

    Application * app = new Application(argc, argv);
    result = app->exec();

    delete app;

    return result;
}