
#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <vector>
#include <string>

namespace DrageEngine
{
    class Cubemap
    {
        public:
            Cubemap();
            ~Cubemap();
        
            bool Load(const std::string &filename);
            unsigned Generate(const std::vector<std::string> &faces);
        
            const std::string& GetName() const;
            unsigned GetID() const;
        
        private:
            std::string m_name;
            unsigned m_id;
    };
}

#endif
