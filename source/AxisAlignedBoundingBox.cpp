
#include <cfloat>

#include "AxisAlignedBoundingBox.h"


AxisAlignedBoundingBox::AxisAlignedBoundingBox()
:   m_urb(QVector3D(-FLT_MAX,-FLT_MAX,-FLT_MAX))
,   m_llf(QVector3D(+FLT_MAX,+FLT_MAX,+FLT_MAX))
,	m_radius(0.f)
{
}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
{
}

const bool AxisAlignedBoundingBox::extend(const QVector3D & vertex)
{
    const QVector3D llf(m_llf);
    const QVector3D urb(m_urb);

    if(vertex.x() < m_llf.x())
        m_llf.setX(vertex.x());
    if(vertex.x() > m_urb.x())
        m_urb.setX(vertex.x());

    if(vertex.y() < m_llf.y())
        m_llf.setY(vertex.y());
    if(vertex.y() > m_urb.y())
        m_urb.setY(vertex.y());

    if(vertex.z() < m_llf.z())
        m_llf.setZ(vertex.z());
    if(vertex.z() > m_urb.z())
        m_urb.setZ(vertex.z());

    const bool extended(urb != m_urb && llf != m_llf);

    if(extended)
    {
        m_center = m_llf + (m_urb - m_llf) * .5f;
        m_radius = (m_urb - m_llf).length() * .5f;
    }
    return extended;
}

const QVector3D & AxisAlignedBoundingBox::center() const
{
    return m_center;
}

const float_t AxisAlignedBoundingBox::radius() const
{
    return m_radius;
}

const QVector3D & AxisAlignedBoundingBox::llf() const
{
    return m_llf;
}

const QVector3D & AxisAlignedBoundingBox::urb() const
{
    return m_urb;
}

const bool AxisAlignedBoundingBox::inside(const QVector3D & vertex) const
{
    return vertex.x() >= m_llf.x() && vertex.x() <= m_urb.x()
        && vertex.y() >= m_llf.y() && vertex.y() <= m_urb.y()
        && vertex.z() >= m_llf.z() && vertex.z() <= m_urb.z();
}

const bool AxisAlignedBoundingBox::outside(const QVector3D & vertex) const
{
    return !inside(vertex);
}
