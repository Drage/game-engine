
#include <numeric>
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
#include "MathUtils.h"

using namespace DrageEngine;

Editor::Editor()
{
    camera = new EditorCamera();
    app->renderer->SetActiveCamera(camera);

    origin = Origin::Center;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(app->window->window, app->window->glContext);
    ImGui_ImplOpenGL3_Init("#version 410 core");
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(250, 150));
    ImGui::GetStyle().WindowRounding = 2.0f;

    std::string fontFile = app->assets->GetAssetPath("OpenSans.ttf");
    io.Fonts->AddFontFromFileTTF(fontFile.c_str(), 28.0f);
    io.FontGlobalScale = 0.5f;
    
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphOffset = ImVec2(0, 1);
    const ImWchar icon_ranges[] = { ICON_MIN, ICON_MAX, 0 };
    std::string iconFontFile = app->assets->GetAssetPath("ForkAwesome.ttf");
    io.Fonts->AddFontFromFileTTF(iconFontFile.c_str(), 28.0f, &config, icon_ranges);
    io.Fonts->Build();

    ImGui::StyleColorsDark();
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.3805f, 0.381f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.2805f, 0.281f, 1.0f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    
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
    camera->Update();

    if (!ImGui::GetIO().WantCaptureMouse)
    {
        static Vector2 mouseDownPosition;
        if (app->input->GetMouseButtonDown(Mouse::Button::LEFT))
            mouseDownPosition = app->input->GetMousePosition();

        if (app->input->GetMouseButtonUp(Mouse::Button::LEFT) && (mouseDownPosition - app->input->GetMousePosition()).Magnitude() < 5)
        {
            Vector2 mousePosition = app->input->GetMousePosition();
            Entity *selected = app->renderer->GetEntityAtScreenPosition(mousePosition);

            Entity *previousSelection = NULL;
            if (selection.size() > 0)
                previousSelection = selection[0];

            selection.clear();

            if (selected)
            {
                Entity *prefabRoot = selected->GetPrefabRoot();
                if (prefabRoot != NULL && prefabRoot != previousSelection)
                    selected = prefabRoot;

                selection.push_back(selected);
                LOG(selected->GetName());
            }
        }
    }

    if (app->input->GetKeyDown(Key::F) && selection.size() > 0)
    {
        Entity *selected = selection[0];
        std::vector<Entity*> entities;
        selected->GetAllChildrenInHierarchy(entities);
        entities.push_back(selected);

        std::vector<Renderable*> renderables;
        app->renderer->GetRenderablesForEntities(entities, renderables);
        if (renderables.size() > 0)
        {
            std::vector<Vector3> positions;
            std::vector<float> radii;
            for (auto renderable : renderables)
            {
                if (!renderable->GetMaterial()->IsUIOverlay())
                {
                    Bounds bounds = renderable->GetMesh()->GetBounds();
                    Transform t = renderable->GetEntity()->GetGlobalTransform();

                    if (origin == Origin::Center)
                        positions.push_back(t.position + t.TransformPoint((bounds.max + bounds.min) / 2));
                    else
                        positions.push_back(t.position);

                    radii.push_back(bounds.radius * Max(t.scale.x, t.scale.y, t.scale.z));
                }
            }
            if (positions.size() > 0)
            {
                Vector3 sumPositions = std::accumulate(positions.begin(), positions.end(), Vector3::ZERO);
                Vector3 center = sumPositions / positions.size();

                float maxDist = 0;
                for (int i = 0; i < radii.size(); i++)
                {
                    float dist = Vector3::Distance(positions[i], center) + radii[i];
                    maxDist = Max(maxDist, dist);
                }

                float zoomFactor = maxDist * 2.5f;
                camera->SetFocus(center, zoomFactor);
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

bool Editor::IsSelected(const Entity* entity) const
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
