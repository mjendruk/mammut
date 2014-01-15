#include "RenderCamera.h"

#include <game_mechanics/GameCamera.h>

RenderCamera::RenderCamera()
:   glowutils::Camera()
{
}

RenderCamera::~RenderCamera()
{
    
}

void RenderCamera::update(GameCamera camera)
{
    setEye(camera.eye());
    setCenter(camera.center());
    setUp(camera.up());
}
