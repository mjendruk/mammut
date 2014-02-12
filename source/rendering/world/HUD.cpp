#include "HUD.h"

#include <algorithm>
#include <glm/gtx/transform.hpp>

#include <logic/world/Mammut.h>
#include "RenderCamera.h"

#include "GameWorldRenderer.h"

HUD::HUD(const RenderCamera & camera, const GameWorldRenderer & renderer)
:   m_camera(camera)
,   m_gameWorldRenderer(renderer)
{
}

HUD::~HUD()
{
}

void HUD::initialize()
{
}

void HUD::paint(const Mammut & mammut)
{
    QString velocityString = QString("Speed: %1").arg(std::max(0, static_cast<int>(-mammut.velocity().z)));
    QString scoreString = QString("Score: %1").arg(std::max(0, static_cast<int>(-mammut.position().z)));
    QString fpsString = QString("%1").arg(m_gameWorldRenderer.fps());
    
    
    glDisable(GL_DEPTH_TEST);
    
    m_textRenderer.paint(velocityString,
                         glm::translate(-0.95f, -0.86f, 0.0f) * glm::scale(1.0f, m_camera.aspectRatio(), 1.0f));
    
    m_textRenderer.paint(scoreString,
                         glm::translate(0.95f, -0.86f, 0.0f) * glm::scale(1.0f, m_camera.aspectRatio(), 1.0f),
                         TextRenderer::kAlignRight);

    m_textRenderer.paint(fpsString,
                         glm::translate(-0.99f, 0.88f, 0.0f) * glm::scale(1.0f, m_camera.aspectRatio(), 1.0f),
                         TextRenderer::kAlignLeft);

    
    glEnable(GL_DEPTH_TEST);
}
