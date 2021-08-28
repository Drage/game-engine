
#include "Component.h"
#include "Entity.h"

using namespace DrageEngine;

void Component::SetEnabled(bool enabled)
{
    this->enabled = enabled;
}

bool Component::IsEnabled() const
{
    return enabled;
}

