#include "HUD.h"

#include <glm/gtx/transform.hpp>

#include "RenderCamera.h"
#include "GameWorldRenderer.h"


const bool HUD::s_showFps = false;

HUD::HUD(const RenderCamera & camera)
:   m_camera(camera)
{
}

void HUD::paint(int velocity, int score, int collectedBoosts, int fps)
{
    const glm::mat4 aspectRatioTransform = glm::scale(1.0f / m_camera.aspectRatio(), 1.0f, 1.0f);
    
    glDisable(GL_DEPTH_TEST);
    
    QString velocityString = QString("Speed: %1").arg(velocity);
    m_textRenderer.paint(velocityString,
                         aspectRatioTransform * glm::translate(m_camera.aspectRatio() * 0.98f, -0.88f, 0.0f),
                         SDFRenderer::kAlignRight);

    m_symbolRenderer.paint("Lightning",
                           collectedBoosts,
                           aspectRatioTransform * glm::translate(m_camera.aspectRatio() * 0.99f, 1.03f, 0.0f) * glm::scale(glm::vec3(0.8f)),
                           SDFRenderer::kAlignRight);
    
    QString scoreString = QString("Score: %1").arg(score);
    m_textRenderer.paint(scoreString,
                         aspectRatioTransform * glm::translate(-m_camera.aspectRatio() * 0.98f, -0.88f, 0.0f),
                         SDFRenderer::kAlignLeft);
    
    if (s_showFps)
    {
        QString fpsString = QString("%1").arg(fps);
        
        m_textRenderer.paint(fpsString,
                             glm::translate(-1.0f, 1.0f, 0.0f) * aspectRatioTransform,
                             SDFRenderer::kAlignLeft);
    
    }

    glEnable(GL_DEPTH_TEST);
}
