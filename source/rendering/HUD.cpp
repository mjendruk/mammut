#include "HUD.h"

#include <algorithm>

#include <glm/gtx/transform.hpp>

#include "../gamelogic/Mammut.h"
#include "RenderCamera.h"

HUD::HUD(const Mammut & mammut, const RenderCamera & camera)
:   m_mammut(mammut)
,   m_camera(camera)
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
    glm::mat4 aspectRatioTransform = glm::scale(1.0f / m_camera.aspectRatio(), 1.0f, 1.0f) * glm::scale(1.1f, 1.1f, 1.0f);
    
    QString velocityString = QString("Speed: %1").arg(std::max(0, static_cast<int>(m_mammut.velocity())));
    QString scoreString = QString("Score: %1").arg(std::max(0, static_cast<int>(-m_mammut.position().z)));
    
    
    glDisable(GL_DEPTH_TEST);
    
    m_stringDrawer.paint(velocityString,
                         aspectRatioTransform * glm::translate(-0.95f, -0.86f, 0.0f));
    
    m_stringDrawer.paint(scoreString,
                         aspectRatioTransform * glm::translate(0.95f, -0.86f, 0.0f),
                         StringDrawer::kAlignRight);
    
    glEnable(GL_DEPTH_TEST);
}
