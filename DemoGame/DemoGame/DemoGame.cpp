
#include "DemoGame.h"
#include <iostream>

using namespace DrageEngine;

void DemoGame::Load()
{
    m_scene = new Scene("Demo.scene");
    m_scene->Start();
}

void DemoGame::Update()
{
    m_scene->Update();
}

void DemoGame::Render(Renderer *renderer)
{
    m_scene->Render(renderer);
}
