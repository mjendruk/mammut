
#include <cmath>
#include <cassert>

#include <QVector3D>
#include <QQuaternion>

#include "MathMacros.h"
#include "Plane3.h"


float pointToPlaneDistance(
    const QVector3D & location
,   const QVector3D & normal
,   const QVector3D & point)
{
    assert(normal.length() == 1.0);

    const float distance = location.length();
    return abs(QVector3D::dotProduct(point, normal) - distance);
}

const QMatrix4x4 rotate(
    const QVector3D & a
,   const QVector3D & b)
{
    const QVector3D anorm(a.normalized());
    const QVector3D bnorm(b.normalized());

    const QVector3D axis = QVector3D::crossProduct(anorm, bnorm);
    const float angle = acos(QVector3D::dotProduct(anorm, bnorm));

    QMatrix4x4 m;
    m.rotate(QQuaternion::fromAxisAndAngle(axis, deg(angle)));

    return m;
}

const QMatrix4x4 transform(
    const QVector3D & location
,   const QVector3D & normal)
{
    QMatrix4x4 m;
    m.translate(location);

    const QVector3D up(0.0, 1.0, 0.0);
    return m * rotate(normal, up);
}

const QVector3D intersection(
	bool & intersects
,   const QVector3D & location
,   const QVector3D & normal
,	const QVector3D & rnear
,	const QVector3D & rfar)
{
	const QVector3D & r0 = rnear; // thats root of the ray
	const QVector3D r = rfar - rnear; // a ray direction

	// intersect with plane in point normal form
	const float rDotN = QVector3D::dotProduct(r, normal);

	intersects = r != QVector3D() && rDotN < 0.; // the later would happen if the intersection is above the "horizon"

	if(!intersects)
		return QVector3D();

    const float t = QVector3D::dotProduct(location - r0, normal) / rDotN;

	return t * r + r0; // retrieve point via the ray
}
