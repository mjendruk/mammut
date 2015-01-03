#include "BunchRenderer.h"

#include <glm/gtx/transform.hpp>

#include <logic/world/tets/BunchOfTets.h>
#include "TetPainter.h"
#include "BunchOfTetsDrawable.h"

BunchRenderer::BunchRenderer()
{

}

BunchRenderer::~BunchRenderer()
{

}

void BunchRenderer::draw(QVector<const BunchOfTets *> bunches, TetPainter & painter)
{
    for (const BunchOfTets * bunch : bunches) {
        if (!drawableForBunchExists(bunch)) {
            m_drawables << new BunchOfTetsDrawable(*bunch);
        }
    }

    for (int i = m_drawables.size() - 1; i >= 0; i--) {
        BunchOfTetsDrawable * drawable = m_drawables[i];
        int index = bunches.indexOf(drawable->bunch());
        if (index == -1) {
            delete m_drawables[i];
            m_drawables.remove(i);
        }
    }

    for (BunchOfTetsDrawable * drawable : m_drawables) {
        painter.paint(*drawable, glm::translate(0.0f, 0.0f, drawable->bunch()->zShift()));
    }
}

void BunchRenderer::reset()
{
    qDeleteAll(m_drawables);
    m_drawables.clear();
}

bool BunchRenderer::drawableForBunchExists(const BunchOfTets * bunch)
{
    for (BunchOfTetsDrawable * drawable : m_drawables)
        if (drawable->bunch() == bunch)
            return true;
    return false;
}
