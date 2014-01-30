#include "ScreenshotBackground.h"

#include <cassert>

#include <glow/Texture.h>
#include <glow/Shader.h>
#include <glow/Program.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/File.h>

ScreenshotBackground::ScreenshotBackground()
{
    initialize();
}

ScreenshotBackground::~ScreenshotBackground()
{
}

void ScreenshotBackground::initialize()
{
    m_program = new glow::Program();
    
    m_program->attach(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/screenshot_background.frag"),
                      glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/screenshot_background.vert"));
    
    m_quad = new glowutils::ScreenAlignedQuad(m_program);
}

void ScreenshotBackground::resize(int width, int height, float devicePixelRatio)
{
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
