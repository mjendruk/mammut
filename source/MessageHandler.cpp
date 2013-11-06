
#include <cassert>

#include <QString>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>
#include <QTextStream>

#include "MessageHandler.h"

MessageHandler * MessageHandler::m_instance = nullptr;


void globalMessageHandler(
    QtMsgType type
,   const QMessageLogContext & context
,   const QString & message)
{
    MessageHandler::instance()->handle(type, context, message);
}

MessageHandler * MessageHandler::instance()
{
    if(!m_instance)
        m_instance = new MessageHandler();

    return m_instance;
}

MessageHandler::MessageHandler(QObject * parent)
:    QObject(parent)
,    m_fstream(nullptr)
{
    printToConsole(true);
    printToFile(true);

    const QFileInfo fi(QCoreApplication::applicationFilePath());
    m_file.setFileName(fi.baseName() + ".log");
}

MessageHandler::~MessageHandler()
{
    if(m_fstream)
    {
        m_fstream->flush();
        delete m_fstream;
    }

    if(m_file.isOpen())
        m_file.close();
}

void MessageHandler::setFileName(const QString & fileName)
{
    QFile &file(instance()->m_file);

    if(file.isOpen())
    {
        file.rename(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
            qWarning("Could not open \"%s\" for message forwarding.", qPrintable(file.fileName()));
    }
    else
        file.setFileName(fileName);
}

const QString MessageHandler::fileName()
{
    return instance()->m_file.fileName();
}

QTextStream * MessageHandler::fstream()
{
    if(m_fstream)
    {
        assert(m_file.isOpen());
        return m_fstream;
    }

    if(!m_file.isOpen() && !m_file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qWarning("Could not open \"%s\" for message forwarding.", qPrintable(m_file.fileName()));
        return nullptr;
    }

    m_fstream = new QTextStream(&m_file);
    return m_fstream;
}

void MessageHandler::printToConsole(
    const bool enable)
{
    m_printToConsole[QtDebugMsg]    = enable;
    m_printToConsole[QtWarningMsg]  = enable;
    m_printToConsole[QtCriticalMsg] = enable;
    m_printToConsole[QtFatalMsg]    = enable;
    m_printToConsole[QtCriticalMsg] = enable;
}

void MessageHandler::printToFile(
    const bool enable)
{
    m_printToFile[QtDebugMsg]    = enable;
    m_printToFile[QtWarningMsg]  = enable;
    m_printToFile[QtCriticalMsg] = enable;
    m_printToFile[QtFatalMsg]    = enable;
    m_printToFile[QtCriticalMsg] = enable;
}

void MessageHandler::setPrintToConsole(
    const bool enable)
{
    instance()->printToConsole(enable);
}

void MessageHandler::setPrintToConsole(
    const QtMsgType type
,   const bool enable)
{
    typedPrintFlags &flags(instance()->m_printToConsole);
    flags[type] = enable;
}

const bool MessageHandler::printsToConsole(const QtMsgType type)
{
    const typedPrintFlags &flags(instance()->m_printToConsole);
    if(!flags.contains(type))
        return false;

    return flags[type];
}

void MessageHandler::setPrintToFile(
    const bool enable)
{
    instance()->printToFile(enable);
}

void MessageHandler::setPrintToFile(
    const QtMsgType type
,   const bool enable)
{
    typedPrintFlags &flags(instance()->m_printToFile);
    flags[type] = enable;
}

const bool MessageHandler::printsToFile(const QtMsgType type)
{
    const typedPrintFlags &flags(instance()->m_printToFile);
    if(!flags.contains(type))
        return false;

    return flags[type];
}

void MessageHandler::handle(
    QtMsgType type
,   const QMessageLogContext & context
,   const QString & message)
{
    // Early exit if no output is required.
    if(!printsToConsole(type)
    && !printsToFile(type))
        return;

    // Prepend appropriate message type hints.

    QString prefix("");

    switch (type) 
    {
    case QtWarningMsg:
        prefix = "WARNING";
        break;

    case QtCriticalMsg:
        prefix = "CRITICAL";
        break;

    case QtFatalMsg:
        prefix = "FATAL";
        break;

    default:
        break;
    }

    QString output(message);

    if(!prefix.isEmpty() && !message.isEmpty())
        output.prepend(prefix + ": ");

    output.append("\n");

    printf(type, output);
}

void MessageHandler::printf(
    QtMsgType type
,   const QString & message)
{
    const QString timestamp(QDateTime::currentDateTime().toString("yy-MM-dd::hh:mm:ss"));

    // Print to Console

    if(m_printToConsole[type])
        fputs(qPrintable(message), stderr);

    // Print to File

    if(m_printToFile[type] && fstream())
    {
        (*fstream()) << QString("[%1] %2").arg(timestamp).arg(message);
        fstream()->flush();
    }    
}
