
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "Editor.h"
#include "Application.h"
#include "Debug.h"

using namespace DrageEngine;

Editor::Editor()
{
    camera = new EditorCamera();
    app->renderer->SetActiveCamera(camera);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(app->window->window, app->window->glContext);
    ImGui_ImplOpenGL3_Init("#version 410 core");
    ImGuiIO& io = ImGui::GetIO();
    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(250, 150));
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 2.0f;

    std::string fontFile = app->assets->GetAssetPath("Roboto.ttf");
    io.Fonts->AddFontFromFileTTF(fontFile.c_str(), 26.0f);
    io.FontGlobalScale = 0.5f;
    
    inspectorWindowOpen = true;
    consoleWindowOpen = true;
}

Editor::~Editor()
{
    delete camera;
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Editor::Update()
{
    if (!ImGui::GetIO().WantCaptureMouse)
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
                selection.push_back(selected);
        }
    }
}

void Editor::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    int dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode;
    int dockspaceId = ImGui::DockSpaceOverViewport(NULL, dockspaceFlags);
    
    if (inspectorWindowOpen)
    {
        ImGui::Begin("Inspector", &inspectorWindowOpen, ImGuiWindowFlags_NoCollapse);
        if (selection.size() > 0)
            ImGui::Text("%s", selection[0]->GetName().c_str());
        ImGui::End();
    }
    
    if (consoleWindowOpen)
    {
        ImGui::Begin("Console", &consoleWindowOpen, ImGuiWindowFlags_NoCollapse);
        
        ImGui::End();
    }
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    auto centeralNode = ImGui::DockBuilderGetCentralNode(dockspaceId);
    float scaleFactor = app->window->scaleFactor;
    int w = centeralNode->Size.x * scaleFactor;
    int h = centeralNode->Size.y * scaleFactor;
    int x = centeralNode->Pos.x * scaleFactor;
    int y = app->window->GetDrawableHeight() - h - centeralNode->Pos.y * scaleFactor;
    app->renderer->ViewportResized(w, h, x, y);
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
