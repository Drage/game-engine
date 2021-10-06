
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "Editor.h"
#include "Application.h"
#include "Debug.h"
#include "Icons.h"
#include "ConsoleWindow.h"
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "AssetsWindow.h"

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
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(250, 150));
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 2.0f;

    std::string fontFile = app->assets->GetAssetPath("Roboto.ttf");
    io.Fonts->AddFontFromFileTTF(fontFile.c_str(), 26.0f);
    io.FontGlobalScale = 0.5f;
    
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphOffset = ImVec2(0, 1);
    const ImWchar icon_ranges[] = { ICON_MIN, ICON_MAX, 0 };
    std::string iconFontFile = app->assets->GetAssetPath("ForkAwesome.ttf");
    io.Fonts->AddFontFromFileTTF(iconFontFile.c_str(), 28.0f, &config, icon_ranges);
    
    windows.push_back(new ConsoleWindow());
    windows.push_back(new HierarchyWindow());
    windows.push_back(new InspectorWindow());
    windows.push_back(new AssetsWindow());
}

Editor::~Editor()
{
    delete camera;
    
    for (std::vector<EditorWindow*>::iterator i = windows.begin(); i != windows.end(); i++)
        delete *i;
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Editor::HandleInput(const SDL_Event* event)
{
    ImGui_ImplSDL2_ProcessEvent(event);
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
            {
                selection.push_back(selected);
                LOG(selected->GetName());
            }
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
    
    for (std::vector<EditorWindow*>::const_iterator i = windows.begin(); i != windows.end(); i++)
        (*i)->Render();
    
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

const std::vector<Entity*>& Editor::GetSelection() const
{
    return selection;
}

void Editor::SetSelection(Entity* entity)
{
    selection.clear();
    selection.push_back(entity);
}
