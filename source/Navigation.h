
#pragma once

#include <QtGlobal>
#include <QObject>

#include <QMatrix4x4>
#include <QMap>

#include <glowutils\AxisAlignedBoundingBox.h>

class QVector2D;
class QVector3D;
class QSize;

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

namespace glow {
    class AbstractCoordinateProvider;
    class Camera;
}



class Navigation: public QObject
{
	Q_OBJECT

	enum InteractionMode
	{
		NoInteraction
	,	PanInteraction
	,	RotateInteraction
	};

public:
	Navigation(glow::Camera & camera);
	virtual ~Navigation();

    void setBoundaryHint(const glow::AxisAlignedBoundingBox & aabb);
    void setCoordinateProvider(glow::AbstractCoordinateProvider * provider);

	virtual void reset(bool update = true);

	// event handling

	virtual void keyPressEvent        (QKeyEvent   * event);
	virtual void keyReleaseEvent      (QKeyEvent   * event);

	virtual void mouseMoveEvent       (QMouseEvent * event);
	virtual void mousePressEvent      (QMouseEvent * event);
	virtual void mouseReleaseEvent    (QMouseEvent * event);
	virtual void mouseDoubleClickEvent(QMouseEvent * event);

	virtual void wheelEvent           (QWheelEvent * event);

protected:
	void panningBegin(const glm::ivec2 & mouse);
	void panningProcess(const glm::ivec2 & mouse);
	void panningEnd();

	void rotatingBegin(const glm::ivec2 & mouse);
	void rotatingProcess(const glm::ivec2 & mouse);
	void rotatingEnd();

    void pan(glm::vec3 t);
    void rotate(float hAngle, float vAngle);

	void scaleAtCenter(float scale);
	void scaleAtMouse(const glm::ivec2 & mouse,	float scale);
	void resetScaleAtMouse(const glm::ivec2 & mouse);

//	void enforceWholeMapVisible(const float offset = 0.08);

	// constraints

	void enforceTranslationConstraints(glm::vec3 & p) const;
	void enforceRotationConstraints(
		float & hAngle
	,	float & vAngle) const;
	void enforceScaleConstraints(
		float & scale
	,	glm::vec3 & i) const;

	// math

	const glm::vec3 mouseRayPlaneIntersection(
        bool & intersects
    ,   const glm::ivec2 & mouse) const;
    const glm::vec3 mouseRayPlaneIntersection(
        bool & intersects
    ,   const glm::ivec2 & mouse
    ,   const glm::vec3 & p0) const;
    const glm::vec3 mouseRayPlaneIntersection(
        bool & intersects
    ,   const glm::ivec2 & mouse
	,	const glm::vec3 & p0
	,	const glm::mat4x4 & viewProjectionInverted) const;

protected:
    glow::Camera & m_camera;
    glow::AxisAlignedBoundingBox m_aabb;

    glow::AbstractCoordinateProvider * m_coordsProvider;

    bool m_rotationHappened;
	InteractionMode m_mode;

    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::mat4x4 m_viewProjectionInverted;

    glm::vec3  m_i0;
    glm::vec3  m_i1;
    bool  m_i0Valid; // stores if initial interaction pick yielded valid depth
    glm::vec2  m_m0;
};
