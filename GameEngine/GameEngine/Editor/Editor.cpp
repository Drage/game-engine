
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
        Vector2 mousePosition = app->input->GetMousePosition();
        Entity *selected = app->renderer->GetEntityAtScreenPosition(mousePosition);

        selection.clear();
        if (selected)
        {
            selection.push_back(selected);
            LOG(selected->GetName());
        }
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

bool Editor::HasSelection() const
{
    return selection.size() > 0;
}
