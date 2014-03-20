#include "Cave.h"


const float Cave::s_caveRadius = 150.0f;
const float Cave::s_caveDeathDistance = 5.0f;

Cave::Cave()
:   m_zShift(0.0f)
{
}

Cave::~Cave()
{
}

float Cave::zShift() const
{
    return m_zShift;
}

void Cave::addZShift(float zShift)
{
    m_zShift += zShift;
}
