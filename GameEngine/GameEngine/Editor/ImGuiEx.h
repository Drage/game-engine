
#ifndef IMGUIEX_H
#define IMGUIEX_H

#include "imgui.h"
#include <vector>
#include <string>

namespace DrageEngine
{
    class ImGuiEx
    {
        public:
            static bool Combo(const char* label, int* selectedIndex, std::vector<std::string>& values);
            static bool ListBox(const char* label, int* selectedIndex, std::vector<std::string>& values);
    };
}

#endif
