#include "AbstractApplication.h"

#include <QFileInfo>

#include "mammut-version.h"

AbstractApplication::AbstractApplication(
    int & argc
,   char ** argv)

:   QGuiApplication(argc, argv)
{
    const QFileInfo fi(QCoreApplication::applicationFilePath());

    QGuiApplication::setApplicationDisplayName(fi.baseName());

    QGuiApplication::setApplicationName(MAMMUT_PROJECT_NAME);
    QGuiApplication::setApplicationVersion(MAMMUT_VERSION);

    QGuiApplication::setOrganizationName(MAMMUT_AUTHOR_ORGANIZATION);
    QGuiApplication::setOrganizationDomain(MAMMUT_AUTHOR_DOMAIN);

    qDebug("%s on Qt-%s\n", qPrintable(title()), qPrintable(qVersion()));
}

AbstractApplication::~AbstractApplication()
{
    qDebug("%s exited\n", qPrintable(title()));
}

const QString AbstractApplication::title() const
{
    return QString("%1 - %2 %3")
        .arg(QGuiApplication::applicationDisplayName())
        .arg(QGuiApplication::applicationName())
        .arg(QGuiApplication::applicationVersion());
}
