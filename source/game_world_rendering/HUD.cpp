#include "HUD.h"

#include <algorithm>
#include <glm/gtx/transform.hpp>

#include <game_mechanics/Mammut.h>
#include "RenderCamera.h"

#include "GameWorldRenderer.h"

HUD::HUD(const Mammut & mammut, const RenderCamera & camera, const GameWorldRenderer & renderer)
:   m_mammut(mammut)
,   m_camera(camera)
,   m_gameWorldRenderer(renderer)
{
}

HUD::~HUD()
{
}

void HUD::initialize()
{
    m_stringDrawer.initialize();
}

void HUD::paint()
{
    QString velocityString = QString("Speed: %1").arg(std::max(0, static_cast<int>(m_mammut.velocity())));
    QString scoreString = QString("Score: %1").arg(std::max(0, static_cast<int>(-m_mammut.position().z)));
    QString fpsString = QString("%1").arg(m_gameWorldRenderer.fps());
    
    
    glDisable(GL_DEPTH_TEST);
    
    m_stringDrawer.paint(velocityString,
                         glm::translate(-0.95f, -0.86f, 0.0f),
                         m_camera.aspectRatio());
    
    m_stringDrawer.paint(scoreString,
                         glm::translate(0.95f, -0.86f, 0.0f),
                         m_camera.aspectRatio(),
                         StringDrawer::kAlignRight);

    m_stringDrawer.paint(fpsString,
                         glm::translate(-0.99f, 0.88f, 0.0f),
                         m_camera.aspectRatio(),
                         StringDrawer::kAlignLeft);

    
    glEnable(GL_DEPTH_TEST);
}
