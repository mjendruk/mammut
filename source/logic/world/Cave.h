#pragma once

class Cave
{

public:
	Cave();
    virtual ~Cave();

    float zShift() const;
    void addZShift(float zShift) const;

public:
    static const float s_caveRadius;
    static const float s_caveDeathDistance;

protected:
    mutable float m_zShift;
};
