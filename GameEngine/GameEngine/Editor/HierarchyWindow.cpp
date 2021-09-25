
#include "imgui.h"
#include "HierarchyWindow.h"
#include "Icons.h"
#include "Application.h"

using namespace DrageEngine;

HierarchyWindow::HierarchyWindow() : EditorWindow("Hierarchy", ICON_SITEMAP)
{
    windowOpen = true;
}

void HierarchyWindow::RenderContent()
{
    const Scene *scene = app->GetActiveScene();
    for (auto i : scene->GetEntities())
    {
        ImGui::Text("%s", i->GetName().c_str());
    }
}
