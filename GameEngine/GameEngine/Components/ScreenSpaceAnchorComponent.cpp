
#include "ScreenSpaceAnchorComponent.h"
#include "Application.h"

using namespace DrageEngine;

void ScreenSpaceAnchorComponent::Init(const ParamList &params)
{
    executeInEditMode = true;
    anchor = params.Get<Vector2>("anchor", Vector2(0));
    Update();
}

void ScreenSpaceAnchorComponent::Update()
{
    Vector2 viewportSize = app->renderer->GetViewportSize();
    float x = anchor.x * viewportSize.x;
    float y = anchor.y * viewportSize.y;
    transform->position = Vector3(x, y, transform->position.z);
}
