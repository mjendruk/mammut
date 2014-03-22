#include "CuboidRenderer.h"

#include <QDebug>

#include <glm/gtx/transform.hpp>

#include <logic/world/Cuboid.h>
#include "Painter.h"
#include "TessCuboidDrawable.h"


CuboidRenderer::CuboidRenderer()
{

}

CuboidRenderer::~CuboidRenderer()
{

}

void CuboidRenderer::draw(QList<Cuboid *> cuboids, Painter & painter)
{
    while (!m_tessCuboidDrawables.isEmpty() && m_tessCuboidDrawables[0]->cuboid() != cuboids[0])
        delete m_tessCuboidDrawables.takeFirst();
    
    int index = m_tessCuboidDrawables.size();
    while (index < cuboids.size() && cuboids[index]->tetsReady()) {
        m_tessCuboidDrawables << new TessCuboidDrawable(cuboids[index]);
        index++;
    }

    for (TessCuboidDrawable * d : m_tessCuboidDrawables) {
        const Cuboid * cuboid = d->cuboid();
        if (cuboid->isDummy())
            continue;
        glm::mat4 cuboidPosition = glm::translate(cuboid->position());
        painter.paint(*d, cuboidPosition, cuboidPosition, cuboid->containsBoost());
    }

    for (int i = m_tessCuboidDrawables.size(); i < cuboids.size(); i++) {
        const Cuboid * cuboid = cuboids[i];
        if (cuboid->isDummy())
            continue;
        painter.paint(m_cuboidDrawable, cuboid->modelTransform(), cuboid->modelTransform(), cuboid->containsBoost());
    }
}
