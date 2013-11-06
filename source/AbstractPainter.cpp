
#include <QStringList>
#include <QDebug>

#include "AbstractPainter.h"
#include "AbstractContext.h"


AbstractPainter::AbstractPainter()
{
}
 
AbstractPainter::~AbstractPainter()
{
}

const bool AbstractPainter::verify() const
{
    bool result = true;

	result &= verifyExtensions();

    return result;
}

const QStringList AbstractPainter::extensions() const
{
    return QStringList();
}

const bool AbstractPainter::verifyExtensions() const
{
    if (!context())
    {
        qWarning("Extensions cannot be veryfied due to uninitialized context.");
        return false;
    }
	if (!context()->isValid())
	{
		qWarning("Extensions cannot be veryfied due to invalid context.");
		return false;
	}

    QStringList unsupported;

    const QStringList extensions(this->extensions());
    foreach(const QString & extension, extensions)
        if(!context()->hasExtension(qPrintable(extension)))
            unsupported << extension;

    if(unsupported.isEmpty())
        return true;

    if(unsupported.size() > 1)
        qWarning("The following mandatory OpenGL extensions are not supported:");
    else
        qWarning("The following mandatory OpenGL extension is not supported:");

    foreach(const QString & extension, unsupported)
        qWarning() << qPrintable(extension);

    qWarning("");

    return false;
}

const bool AbstractPainter::isValid() const
{
    return context() != nullptr;
}

void AbstractPainter::show()
{
}

void AbstractPainter::hide()
{
}