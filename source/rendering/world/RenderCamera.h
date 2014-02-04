#pragma once

#include <glowutils/Camera.h>

class GameCamera;


class RenderCamera : public glowutils::Camera
{
public:
    RenderCamera();

    virtual ~RenderCamera();

    void update(const GameCamera & camera);
};