
#include "Component.h"
#include "GameObject.h"

using namespace DrageEngine;

void Component::SetEnabled(bool enabled)
{
    this->enabled = enabled;
}

bool Component::IsEnabled() const
{
    return enabled;
}

