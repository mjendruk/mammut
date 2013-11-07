
#pragma once

#include "FileAssociatedAsset.h"
#include <QOpenGLShader>
#include <QMap>
#include <QList>
#include <QQueue>

class QOpenGLShaderProgram;
class QFileSystemWatcher;

/** ToDo: does not work for multiple contexts yet... 
*/

class FileAssociatedShader : public FileAssociatedAsset
{
public:
    static QOpenGLShader * getOrCreate(
        QOpenGLShader::ShaderType type
    ,   const QString & fileName
    ,   QOpenGLShaderProgram & program);


    /** This processes the queue of files with shaders associated by 
        recompiling those and relinking related programs.
        Note: This requires context to be made current!
    */
    static QList<QOpenGLShaderProgram *> process();

protected slots:
    void shaderDestroyed(QObject * object);
    void programDestroyed(QObject * object);

private:
    FileAssociatedShader();
    ~FileAssociatedShader();

protected:
    static QMap<QString, QOpenGLShader *> s_shaderByFilePath;
    static QMultiMap<QOpenGLShader *, QOpenGLShaderProgram *> s_programsByShader;
};
