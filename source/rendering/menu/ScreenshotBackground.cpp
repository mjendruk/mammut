#include "ScreenshotBackground.h"

#include <cassert>

#include <glow/Texture.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/global.h>

ScreenshotBackground::ScreenshotBackground()
{
    initialize();
}

ScreenshotBackground::~ScreenshotBackground()
{
}

void ScreenshotBackground::initialize()
{
    m_quad = new glowutils::ScreenAlignedQuad(
        glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/shaders/screenshot_background.frag"));
}

void ScreenshotBackground::render(float devicePixelRatio)
{
    assert(m_screenshot.get() != nullptr);
    
    m_quad->draw();
}

void ScreenshotBackground::setScreenshot(glow::Texture * screenshot)
{
    m_screenshot = screenshot;
    m_quad->setTexture(m_screenshot);
}
