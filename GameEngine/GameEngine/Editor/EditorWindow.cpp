
#include "imgui.h"
#include "EditorWindow.h"
#include "StringUtils.h"

using namespace DrageEngine;

EditorWindow::EditorWindow(const std::string &name, const char *icon)
{
    windowName = String::Join({ icon, "  ", name });
    windowOpen = false;
    hasMenuBar = false;
}

void EditorWindow::Render()
{
    if (windowOpen)
    {
        int flags = ImGuiWindowFlags_NoCollapse;
        if (hasMenuBar) flags |= ImGuiWindowFlags_MenuBar;
        
        ImGui::Begin(windowName.c_str(), &windowOpen, flags);
        RenderContent();
        ImGui::End();
    }
}

void EditorWindow::RenderContent()
{
    
}
