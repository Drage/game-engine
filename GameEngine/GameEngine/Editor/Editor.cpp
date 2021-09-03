
#include "Editor.h"
#include "Application.h"
#include "Debug.h"
#include "Random.h"

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
//        app->renderer->SetEditorSelectionModeEnabled(true);
//
//        const Scene* scene = app->GetActiveScene();
//        app->renderer->Render();
//
//        Vector2 mousePosition = app->input->GetMousePosition();
//        int pickedIndex = app->renderer->GetSceneObjectIndexAtScreenPosition(mousePosition);
//
//        selection.clear();
//        Entity* selected = scene->GetEntityByIndex(pickedIndex);
//        if (selected)
//            selection.push_back(selected);
//
//        app->renderer->SetEditorSelectionModeEnabled(false);
        
        selection.clear();
        selection.push_back(app->GetActiveScene()->GetEntityByIndex(Random::Int(0, 15)));
    }
}

bool Editor::IsSelected(Entity* entity) const
{
    for (std::vector<Entity*>::const_iterator i = selection.begin(); i != selection.end(); i++)
    {
        if ((*i) == entity)
            return true;
    }
    return false;
}
