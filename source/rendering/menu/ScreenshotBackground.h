#pragma once

#include <glow/ref_ptr.h>

#include "AbstractBackground.h"

namespace glow
{
    class Texture;
    class Program;
}

namespace glowutils
{
    class ScreenAlignedQuad;
}

class ScreenshotBackground : public AbstractBackground
{
public:
    ScreenshotBackground();
    virtual ~ScreenshotBackground();

    void resize(int width, int height) override;
    void render(float devicePixelRatio) override;

    void setScreenshot(glow::Texture * screenshot);

protected:
    void initialize();

protected:
    glow::ref_ptr<glow::Texture> m_screenshot;
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;

};
