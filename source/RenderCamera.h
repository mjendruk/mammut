#pragma once

#include <glowutils/Camera.h>

class GameCamera;


class RenderCamera : public glow::Camera
{
public:
    RenderCamera();

    virtual ~RenderCamera();

    void update(GameCamera camera);
};