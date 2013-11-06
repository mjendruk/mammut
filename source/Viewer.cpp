
#include <QOpenGLContext>
#include <QSettings>

#include "ui_Viewer.h"

#include "Viewer.h"

namespace 
{
    const QString SETTINGS_GEOMETRY ("Geometry");
    const QString SETTINGS_STATE    ("State");
}


Viewer::Viewer(
    QWidget * parent,
    Qt::WindowFlags flags)

:   QMainWindow(parent, flags)
,   m_ui(new Ui_Viewer)
{
    m_ui->setupUi(this);

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;

    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s.value(SETTINGS_STATE).toByteArray());
};

Viewer::~Viewer()
{
	QSettings s;
	s.setValue(SETTINGS_GEOMETRY, saveGeometry());
	s.setValue(SETTINGS_STATE, saveState());
}
