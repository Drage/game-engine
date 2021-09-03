
#include "DemoGame.h"
#include <iostream>

using namespace DrageEngine;

void DemoGame::Load()
{
    scene = new Scene("Demo.scene");
}

void DemoGame::Start()
{
    scene->Start();
}

void DemoGame::Update()
{
    scene->Update();
}
