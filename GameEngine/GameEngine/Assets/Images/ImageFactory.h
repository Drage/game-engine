
#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include <string>
#include "Factory.h"
#include "Image.h"

namespace DrageEngine
{
    class ImageFactory
    {
        public:
            ImageFactory();
            Image* Create(const std::string &extension);
            bool ExtensionSupported(const std::string &extension) const;
        
        private:
            Factory<Image, std::string> factory;
    };
}

#endif
