
#include "Component.h"
#include "GameObject.h"

using namespace DrageEngine;

void Component::SetEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool Component::IsEnabled() const
{
    return m_enabled;
}

