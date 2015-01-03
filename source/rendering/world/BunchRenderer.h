#pragma once

#include <QVector>

class BunchOfTets;
class BunchOfTetsDrawable;
class TetPainter;

class BunchRenderer
{
public:
    BunchRenderer();
    ~BunchRenderer();

    void draw(QVector<const BunchOfTets *> bunches, TetPainter & painter);

    void reset();

protected:
    bool drawableForBunchExists(const BunchOfTets * bunch);

protected:
    QVector<BunchOfTetsDrawable *> m_drawables;
};
