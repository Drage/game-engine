
#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <vector>
#include <string>
#include "EditorWindow.h"

namespace DrageEngine
{
    class ConsoleWindow : public EditorWindow
    {
        public:
            ConsoleWindow();
        
        private:
            void RenderContent();
            void RenderToolbar();
            void RenderLogList();
        
            static const int maxSearchInputLength = 128;
            const char* searchPlaceholder = "Search...";
            char searchInput[maxSearchInputLength];
            bool searchFilterApplied;
        
            std::vector<std::string> sourceFilterOptions;
            int selectedSourceFilter;
        
            const char *typeFilterOptions[4] = { "All Types", "Info", "Warning", "Error" };
            int selectedTypeFilter;
    };
}

#endif
