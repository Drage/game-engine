
#include "imgui.h"
#include "InspectorWindow.h"
#include "Icons.h"
#include "Debug.h"
#include "Application.h"

using namespace DrageEngine;

InspectorWindow::InspectorWindow() : EditorWindow("Inspector", ICON_INFO_CIRCLE)
{
    windowOpen = true;
}

void InspectorWindow::RenderContent()
{
    if (app->editor->GetSelection().size() > 0)
        ImGui::Text("%s", app->editor->GetSelection()[0]->GetName().c_str());
}
