
#pragma once

#include <memory>

#include <QMainWindow>


class Ui_Viewer;
class QSurfaceFormat;

class Viewer : public QMainWindow
{
public:
    Viewer(
        QWidget * parent = nullptr
    ,   Qt::WindowFlags flags = NULL);

    virtual ~Viewer();

protected:
	const std::unique_ptr<Ui_Viewer> m_ui;
};
