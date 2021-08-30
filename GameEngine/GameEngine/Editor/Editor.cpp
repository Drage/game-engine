
#include "Editor.h"
#include "Application.h"
#include "Debug.h"

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
    
    if (app->input->GetMouseButtonDown(Mouse::Button::LEFT))
    {
        app->renderer->SetEditorSelectionModeEnabled(true);
        
        const Scene* scene = app->GetActiveScene();
        app->renderer->BeginRender();
        scene->Render(app->renderer);
        app->renderer->EndRender();
        
        Vector2 mousePosition = app->input->GetMousePosition();
        int pickedIndex = app->renderer->GetSceneObjectIndexAtScreenPosition(mousePosition);
        
        Entity* selected = scene->GetEntityByIndex(pickedIndex);
        if (selected)
            LOG(selected->GetName());
        
        app->renderer->SetEditorSelectionModeEnabled(false);
    }
}
