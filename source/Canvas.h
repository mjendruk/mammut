
#pragma once

#include <QGLWidget>
#include <QList>

#include "Format.h"

class QBasicTimer;
class QTimerEvent;

class AbstractPainter;
class DebugPainter;
class PrintPainter;


/** Canvas is a wrapper QGLWidget that forwards all events to the attached contexts
    and uses one of them as activly visible context.

    An QGLWidget is choosen for now, since using contexts dettached from a widget
    is working for all other rendering apis, except with qt.
    see http://qt-project.org/forums/viewthread/30192/

    So the QGLWidget is still ok, since at least the always available debug and print
    painters can at least rely on that context, and the format description is done
    via QGLFormat now instead of QSurface and QSurfaceFormat.
*/
class Canvas : public QGLWidget
{
public:
    Canvas(
		const Format & format ///> setFormat is deprecated in QGLWidget, and might be used in glwhites future architecture
	,	QWidget * parent = nullptr);
    virtual ~Canvas();

	//void setFormat(const Format & format);
	const Format & format() const;

    const bool attachPainter(AbstractPainter * painter);
    const bool dettachPainter(AbstractPainter * painter);

    const bool setActivePainter(AbstractPainter * painter);

	void setContinuousRepaint(const bool enable);
	bool continuousRepaint() const;

protected:
	virtual void initializeGL();
	virtual void paintGL();

	virtual void showEvent(QShowEvent * event);
	virtual void hideEvent(QHideEvent * event);

	virtual void resizeEvent(QResizeEvent * event);
	//virtual void paintEvent(QPaintEvent * event);

	void timerEvent(QTimerEvent * event);
    void updateViewport() const;

protected:
	// Note: these two functions should be provided by future QtContext class
	static const QGLFormat fromFormat(const Format & format);
	static const Format toFormat(const QGLFormat & format);

protected:
	// Note: temporary solution only, this should be handled by each painter and its context.
	Format m_requested; 

    typedef QList<AbstractPainter *> Painters;
    Painters m_painters;

    AbstractPainter * m_activePainter;

    QBasicTimer * m_timer;

    bool m_initialized;    
    bool m_continuousRepaint;

};
