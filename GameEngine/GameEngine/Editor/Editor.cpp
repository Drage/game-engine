
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
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

    std::string fontFile = app->assets->GetAssetPath("Roboto.ttf");
    ImGui::GetIO().Fonts->AddFontFromFileTTF(fontFile.c_str(), 26.0f);
    ImGui::GetIO().FontGlobalScale = 0.5f;
    
    //ImGui::GetStyle().ScaleAllSizes(2)
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
    
    ImGui::Begin("Test Window", NULL);
    if (selection.size() > 0)
        ImGui::Text("%s", selection[0]->GetName().c_str());
    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
