#include "AbstractApplication.h"

#include <QFileInfo>
#include <QStringList>

#include "mammut-version.h"
#include "MessageHandler.h"

AbstractApplication::AbstractApplication(
    int & argc
,   char ** argv)

:   QApplication(argc, argv)
{
    const QFileInfo fi(QCoreApplication::applicationFilePath());

    QApplication::setApplicationDisplayName(fi.baseName());

    QApplication::setApplicationName(MAMMUT_PROJECT_NAME);
    QApplication::setApplicationVersion(MAMMUT_VERSION);

    QApplication::setOrganizationName(MAMMUT_AUTHOR_ORGANIZATION);
    QApplication::setOrganizationDomain(MAMMUT_AUTHOR_DOMAIN);

    qInstallMessageHandler(globalMessageHandler);

    qDebug("%s on Qt-%s\n", qPrintable(title()), qPrintable(qVersion()));
}

AbstractApplication::~AbstractApplication()
{
    qDebug("%s exited\n", qPrintable(title()));
}

const QString AbstractApplication::title() const
{
    return QString("%1 - %2 %3")
        .arg(QApplication::applicationDisplayName())
        .arg(QApplication::applicationName())
        .arg(QApplication::applicationVersion());
}
