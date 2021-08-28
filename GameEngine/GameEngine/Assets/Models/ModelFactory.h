
#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include <string>
#include "Factory.h"
#include "Model.h"

namespace DrageEngine
{
    class ModelFactory
    {
        public:
            ModelFactory();
            Model* Create(const std::string &extension);
            bool ExtensionSupported(const std::string &extension) const;
        
        private:
            Factory<Model, std::string> factory;
    };
}

#endif
