
#pragma once

#include <QObject>
#include <QMap>
#include <QFile>


class QTextStream;

void globalMessageHandler(
    QtMsgType type
,   const QMessageLogContext & context
,   const QString & message);


class MessageHandler : public QObject
{
    Q_OBJECT

public:
    static MessageHandler * instance();

    static void setPrintToConsole(const bool enable);
    static void setPrintToConsole(
        const QtMsgType type
    ,   const bool enable);    static const bool printsToConsole(const QtMsgType type);

    static void setPrintToFile(const bool enable);
    static void setPrintToFile(
        const QtMsgType type
    ,   const bool enable);
    static const bool printsToFile(const QtMsgType type);

    static void setFileName(const QString & fileName);
    static const QString fileName();

private:
    explicit MessageHandler(QObject * parent = nullptr);
    virtual ~MessageHandler();

protected:
    void printToConsole(const bool enable);
    void printToFile(const bool enable);

    QTextStream * fstream();

public:
    void handle(
        QtMsgType type
    ,   const QMessageLogContext & context
    ,   const QString & message);

    void printf(
        QtMsgType type
    ,   const QString & message);

protected:
    static MessageHandler * m_instance;

protected:

    typedef QMap<QtMsgType, bool> typedPrintFlags;
    typedPrintFlags m_printToConsole;
    typedPrintFlags m_printToFile;

    QFile m_file;
    QTextStream * m_fstream;
};