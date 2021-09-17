
#include "ScreenSpaceAnchorComponent.h"
#include "Application.h"

using namespace DrageEngine;

void ScreenSpaceAnchorComponent::Init(const ParamList &params)
{
    anchor = params.Get<Vector2>("anchor", Vector2(0));
    Update();
}

void ScreenSpaceAnchorComponent::Update()
{
    int screenWidth = app->window->GetWidth();
    int screenHeight = app->window->GetHeight();
    float x = anchor.x * screenWidth;
    float y = anchor.y * screenHeight;
    transform->position = Vector3(x, y, transform->position.z);
}
