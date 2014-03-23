#include "HUD.h"

#include <glm/gtx/transform.hpp>

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

void HUD::paint(int velocity, int score, int collectedBoosts)
{
    QString velocityString = QString("Speed: %1").arg(velocity);
    QString scoreString = QString("Score: %1").arg(score);
    QString fpsString = QString("%1").arg(m_gameWorldRenderer.fps());
    
    const glm::mat4 aspectRatioTransform = glm::scale(1.0f / m_camera.aspectRatio(), 1.0f, 1.0f);
    
    glDisable(GL_DEPTH_TEST);
    
    m_textRenderer.paint(velocityString,
                         aspectRatioTransform * glm::translate(m_camera.aspectRatio() * 0.98f, -0.88f, 0.0f),
                         SDFRenderer::kAlignRight);

    m_symbolRenderer.paint("Lightning",
                         collectedBoosts,
                         aspectRatioTransform * glm::translate(m_camera.aspectRatio() * 0.99f, 1.03f, 0.0f) * glm::scale(glm::vec3(0.8f)),
                         SDFRenderer::kAlignRight);

    m_textRenderer.paint(scoreString,
                         aspectRatioTransform * glm::translate(-m_camera.aspectRatio() * 0.98f, -0.88f, 0.0f),
                         SDFRenderer::kAlignLeft);

    m_textRenderer.paint(fpsString,
                         glm::translate(-1.0f, 1.0f, 0.0f) * aspectRatioTransform,
                         SDFRenderer::kAlignLeft);

    glEnable(GL_DEPTH_TEST);
}
