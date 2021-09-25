
#include "ImGuiEx.h"

using namespace DrageEngine;

static auto vectorGetter = [](void* vec, int idx, const char** out_text)
{
    auto& vector = *static_cast<std::vector<std::string>*>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
    *out_text = vector.at(idx).c_str();
    return true;
};

bool ImGuiEx::Combo(const char* label, int* selectedIndex, std::vector<std::string>& values)
{
    if (values.empty()) { return false; }
    return ImGui::Combo(label, selectedIndex, vectorGetter, static_cast<void*>(&values), (int)values.size());
}

bool ImGuiEx::ListBox(const char* label, int* selectedIndex, std::vector<std::string>& values)
{
    if (values.empty()) { return false; }
    return ImGui::ListBox(label, selectedIndex, vectorGetter, static_cast<void*>(&values), (int)values.size());
}
