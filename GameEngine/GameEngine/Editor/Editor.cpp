
#include "Editor.h"
#include "Application.h"

using namespace DrageEngine;

Editor::Editor()
{
    camera = new EditorCamera();
    app->renderer->SetActiveCamera(camera);
}

Editor::~Editor()
{
    delete camera;
}

void Editor::Update()
{
    camera->Update();
}
