#pragma once

class Cave
{
public:
    Cave();
    ~Cave();

    float zShift() const;
    void addZShift(float zShift);

public:
    static const float s_caveRadius;
    static const float s_caveDeathDistance;

protected:
    float m_zShift;
};
