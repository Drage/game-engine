
#include <GameEngine.h>
#include "DemoGame.h"
#include "Rotate.h"
#include "CameraController.h"
#include "Racer.h"
#include "Camera3rdPerson.h"
#include "Clouds.h"

using namespace DrageEngine;

void RegisterCustomComponents()
{
    app->assets->RegisterComponent<Rotate>("Rotate");
    app->assets->RegisterComponent<CameraController>("CameraController");
    app->assets->RegisterComponent<Racer>("Racer");
    app->assets->RegisterComponent<Camera3rdPerson>("Camera3rdPerson");
    app->assets->RegisterComponent<Clouds>("Clouds");
}

int main(int argc, const char * argv[])
{
    if (!app->Init())
        return 1;
    
    RegisterCustomComponents();

    DemoGame game;
    app->Run(&game);

    return 0;
}
