
#pragma once

class QPointF;
class QMatrix4x4;

class AbstractCoordinateProvider
{
public:
    virtual const float depthAt(const QPointF & windowCoordinates) = 0;

    virtual const QVector3D objAt(
        const QPointF & windowCoordinates) = 0;

	virtual const QVector3D objAt(
        const QPointF & windowCoordinates
	,	const float depth) = 0;

	virtual const QVector3D objAt(
		const QPointF & windowCoordinates
	,	const float depth
	,	const QMatrix4x4 & viewProjectionInverted) = 0;
};
