
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
    RenderEntities(scene->GetEntities());
}

void HierarchyWindow::RenderEntities(const std::vector<Entity*> entities)
{
    for (auto i : entities)
    {
        int flags = ImGuiTreeNodeFlags_OpenOnArrow;
        
        if (i->GetChildren().size() == 0)
            flags |= ImGuiTreeNodeFlags_NoArrow;
        
        if (app->editor->IsSelected(i))
            flags |= ImGuiTreeNodeFlags_Selected;
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
        bool open = ImGui::TreeNodeEx(i->GetName().c_str(), flags);
        ImGui::PopStyleVar();
        
        if (ImGui::IsItemClicked())
            app->editor->SetSelection(i);
        
        if (open)
        {
            RenderEntities(i->GetChildren());
            ImGui::TreePop();
        }
    }
}
