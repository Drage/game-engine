
#include "Component.h"
#include "Entity.h"

using namespace DrageEngine;

Component::Component()
{
    enabled = true;
    executeInEditMode = false;
}

void Component::SetEnabled(bool enabled)
{
    this->enabled = enabled;
}

bool Component::IsEnabled() const
{
    return enabled;
}

bool Component::ExecuteInEditMode() const
{
    return executeInEditMode;
}
