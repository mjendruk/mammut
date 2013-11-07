#pragma once

#include <QMatrix4x4>

class QVector3D;

float pointToPlaneDistance(
    const QVector3D & location
,   const QVector3D & normal
,   const QVector3D & point);

const QMatrix4x4 rotate(
    const QVector3D & a
,   const QVector3D & b);

const QMatrix4x4 transform(
    const QVector3D & location
,   const QVector3D & normal);

const QVector3D intersection(
	bool & intersects
,   const QVector3D & location
,   const QVector3D & normal
,	const QVector3D & rnear
,	const QVector3D & rfar);
