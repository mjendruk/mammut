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
    QString velocityString = QString("Speed: %1").arg(max(0, static_cast<int>(mammut.velocity())));
    QString scoreString = QString("Score: %1").arg(max(0, static_cast<int>(-mammut.position().z)));
    QString fpsString = QString("%1").arg(m_gameWorldRenderer.fps());
    
    
    glDisable(GL_DEPTH_TEST);
    
    m_stringDrawer.paint(velocityString,
                         glm::translate(-0.95f, -0.86f, 0.0f) * glm::scale(1.0f, m_camera.aspectRatio(), 1.0f));
    
    m_stringDrawer.paint(scoreString,
                         glm::translate(0.95f, -0.86f, 0.0f) * glm::scale(1.0f, m_camera.aspectRatio(), 1.0f),
                         StringDrawer::kAlignRight);

    m_stringDrawer.paint(fpsString,
                         glm::translate(-0.99f, 0.88f, 0.0f) * glm::scale(1.0f, m_camera.aspectRatio(), 1.0f),
                         StringDrawer::kAlignLeft);

    
    glEnable(GL_DEPTH_TEST);
}
