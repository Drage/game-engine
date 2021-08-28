
#include <algorithm>
#include "ImageFactory.h"
#include "ImagePNG.h"
#include "ImageTGA.h"
#include "ImageBMP.h"
#include "ImagePCX.h"
#include "ImageJPG.h"

using namespace DrageEngine;

ImageFactory::ImageFactory()
{
    // Register image types with factory
    factory.Register<ImagePNG>("png");
    factory.Register<ImageTGA>("tga");
    factory.Register<ImageBMP>("bmp");
    factory.Register<ImagePCX>("pcx");
    factory.Register<ImageJPG>("jpg");
}

Image* ImageFactory::Create(const std::string &extension)
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    return factory.Create(ext);
}

bool ImageFactory::ExtensionSupported(const std::string &extension) const
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    return factory.IsSupported(ext);
}
