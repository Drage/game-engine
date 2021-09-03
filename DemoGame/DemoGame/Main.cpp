
#include <GameEngine.h>
#include "DemoGame.h"
#include "Rotate.h"
#include "Racer.h"
#include "Camera3rdPerson.h"
#include "CloudGenerator.h"
#include "Cloud.h"

using namespace DrageEngine;

void RegisterCustomComponents()
{
    app->assets->RegisterComponent<Rotate>("Rotate");
    app->assets->RegisterComponent<Racer>("Racer");
    app->assets->RegisterComponent<Camera3rdPerson>("Camera3rdPerson");
    app->assets->RegisterComponent<CloudGenerator>("Clouds");
    app->assets->RegisterComponent<Cloud>("Cloud");
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
