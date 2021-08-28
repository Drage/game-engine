
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
    m_factory.Register<ImagePNG>("png");
    m_factory.Register<ImageTGA>("tga");
    m_factory.Register<ImageBMP>("bmp");
    m_factory.Register<ImagePCX>("pcx");
    m_factory.Register<ImageJPG>("jpg");
}

Image* ImageFactory::Create(const std::string &extension)
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    return m_factory.Create(ext);
}

bool ImageFactory::ExtensionSupported(const std::string &extension) const
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    return m_factory.IsSupported(ext);
}
