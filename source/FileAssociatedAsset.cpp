#include <QFileSystemWatcher>
#include <QMap>
#include <QList>
#include <QQueue>


#include "FileAssociatedAsset.h"

QQueue<QString> FileAssociatedAsset::s_queue;

FileAssociatedAsset * FileAssociatedAsset::s_instance = nullptr;

FileAssociatedAsset::FileAssociatedAsset()
: m_fileSystemWatcher(new QFileSystemWatcher())
{
    connect(m_fileSystemWatcher, &QFileSystemWatcher::fileChanged
        , this, &FileAssociatedAsset::fileChanged);
}

FileAssociatedAsset::~FileAssociatedAsset()
{
    delete m_fileSystemWatcher;
    s_queue.clear();
}

FileAssociatedAsset * FileAssociatedAsset::instance()
{
    if (!s_instance)
        s_instance = new FileAssociatedAsset();

    return s_instance;
}

void FileAssociatedAsset::fileChanged(const QString & filePath)
{
    s_queue.append(filePath);
}

QFileSystemWatcher * const FileAssociatedAsset::fileSystemWatcher()
{
	return instance()->m_fileSystemWatcher;
}

void FileAssociatedAsset::addResourcePath(const QString & filePath)
{
	m_fileSystemWatcher->addPath(filePath);
}
