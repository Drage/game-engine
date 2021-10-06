
#include "imgui.h"
#include <set>
#include "ConsoleWindow.h"
#include "Icons.h"
#include "Debug.h"
#include "StringUtils.h"
#include "ImGuiEx.h"
#include "ArrayUtils.h"

using namespace DrageEngine;

ConsoleWindow::ConsoleWindow() : EditorWindow("Console", ICON_TERMINAL)
{
    windowOpen = true;
    hasMenuBar = true;
    
    strcpy(searchInput, searchPlaceholder);
    searchFilterApplied = false;
    
    selectedSourceFilter = 0;
    selectedTypeFilter = 0;
}

void ConsoleWindow::RenderContent()
{
    RenderToolbar();
    RenderLogList();

    // Auto scroll to end
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
}

void ConsoleWindow::RenderToolbar()
{
    ImGui::BeginMenuBar();
    
    // Text search
    ImGui::Text(ICON_SEARCH);
    ImGui::PushItemWidth(ImGui::GetWindowWidth() > 305 ? 200 : ImGui::GetWindowWidth() - 105);
    ImGui::InputText("##ConsoleSearchLog", searchInput, maxSearchInputLength, ImGuiInputTextFlags_AutoSelectAll);
    ImGui::PopItemWidth();
    if (strcmp(searchInput, "") == 0)
    {
        strcpy(searchInput, searchPlaceholder);
        searchFilterApplied = false;
    }
    else if (strcmp(searchInput, searchPlaceholder) == 0)
        searchFilterApplied = false;
    else
        searchFilterApplied = true;
    
    // Filter on source
    if (ImGui::GetWindowWidth() > 475)
    {
        const Debug::LogHistory& logHistory = Debug::GetLogHistory();
        std::set<std::string> sourcesSet;
        for (Debug::LogHistory::const_iterator i = logHistory.begin(); i != logHistory.end(); i++)
            sourcesSet.insert(i->originFile);
        sourceFilterOptions.clear();
        sourceFilterOptions.push_back("All Sources");
        for (std::set<std::string>::const_iterator i = sourcesSet.begin(); i != sourcesSet.end(); i++)
            sourceFilterOptions.push_back(*i);
        ImGui::PushItemWidth(160);
        ImGuiEx::Combo("##ConsoleFilterSource", &selectedSourceFilter, sourceFilterOptions);
        ImGui::PopItemWidth();
    }
    
    // Filter on type
    if (ImGui::GetWindowWidth() > 585)
    {
        ImGui::PushItemWidth(100);
        ImGui::Combo("##ConsoleFilterType", &selectedTypeFilter, typeFilterOptions, Array::Length(typeFilterOptions));
        ImGui::PopItemWidth();
    }
    
    // Clear button
    ImGui::SameLine(ImGui::GetWindowWidth() - 81);
    if (ImGui::Button(String::Join({ ICON_TRASH_O, "  Clear" }).c_str()))
        Debug::ClearLogHistory();
    
    ImGui::EndMenuBar();
}

void ConsoleWindow::RenderLogList()
{
    auto logHistory = Debug::GetLogHistory();
    for (Debug::LogHistory::const_iterator i = logHistory.begin(); i != logHistory.end(); i++)
    {
        if (searchFilterApplied && !String::ContainsIgnoreCase(i->message, std::string(searchInput)))
            continue;
            
        if (selectedSourceFilter != 0 && i->originFile != sourceFilterOptions[selectedSourceFilter])
            continue;
        
        if (selectedTypeFilter != 0 && i->type != selectedTypeFilter - 1)
            continue;
        
        std::string nodeId = "##" + std::to_string(i->id);
        ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(0, 0, 0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 6.0f });
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5.0f, 0.0f });
        bool open = ImGui::TreeNodeEx(nodeId.c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoArrow);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5.0f, open ? 5.0f : 0.0f });
        ImGui::SameLine(10);
        switch (i->type)
        {
            case Debug::LogType::INFO:
                ImGui::Text("%s", ICON_INFO_CIRCLE);
                break;

            case Debug::LogType::WARNING:
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
                ImGui::Text("%s", ICON_EXCLAMATION_TRIANGLE);
                ImGui::PopStyleColor();
                break;

            case Debug::LogType::ERROR:
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                ImGui::Text("%s", ICON_WINDOW_CLOSE);
                ImGui::PopStyleColor();
                break;
        }

        ImGui::SameLine(32);
        ImGui::Text("%s", i->message.c_str());

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 150));
        ImGui::Text("[%s:%d]", i->originFile.c_str(), i->originLine);
        ImGui::PopStyleColor();
        
        if (ImGui::GetWindowWidth() > 600)
        {
            ImGui::SameLine(ImGui::GetWindowWidth() - 88);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 150));
            ImGui::Text("%s", i->timestamp.c_str());
            ImGui::PopStyleColor();
        }
        ImGui::PopStyleVar();
        
        if (open)
        {
            ImGui::Indent();
            std::string backtrace = i->backtrace;
            ImGui::Text("%s", String::ReplaceAll(backtrace, " ↳ ", "").c_str());
            ImGui::Unindent();
            ImGui::TreePop();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5.0f, 0.0f });
        ImGui::Separator();
        ImGui::PopStyleVar();
    }
}
