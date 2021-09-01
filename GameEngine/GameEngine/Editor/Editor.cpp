
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
    
    static Vector2 mouseDownPosition;
    if (app->input->GetMouseButtonDown(Mouse::Button::LEFT))
        mouseDownPosition = app->input->GetMousePosition();

    if (app->input->GetMouseButtonUp(Mouse::Button::LEFT) && (mouseDownPosition - app->input->GetMousePosition()).Magnitude() == 0)
    {
        app->renderer->SetEditorSelectionModeEnabled(true);
        
        const Scene* scene = app->GetActiveScene();
        app->renderer->BeginRender();
        scene->Render(app->renderer);
        app->renderer->EndRender();
        
        Vector2 mousePosition = app->input->GetMousePosition();
        int pickedIndex = app->renderer->GetSceneObjectIndexAtScreenPosition(mousePosition);
        
        selection.clear();
        Entity* selected = scene->GetEntityByIndex(pickedIndex);
        if (selected)
            selection.push_back(selected);
        
        app->renderer->SetEditorSelectionModeEnabled(false);
    }
}

const std::vector<Entity*>& Editor::GetSelection() const
{
    return selection;
}

const std::vector<int> Editor::GetSelectionSceneIndexes() const
{
    std::vector<int> indexes;
    const Scene* scene = app->GetActiveScene();
    
    for (std::vector<Entity*>::const_iterator i = selection.begin(); i != selection.end(); i++)
        indexes.push_back(scene->GetIndexOfEntity(*i));
    
    return indexes;
}
