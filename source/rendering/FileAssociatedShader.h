#pragma once

#include <GL/glew.h>

#include <QMap>
#include <QList>
#include <QQueue>

#include "FileAssociatedAsset.h"

class QFileSystemWatcher;

namespace glow
{
    class Program;
    class Shader;
}

/** ToDo: does not work for multiple contexts yet... 
*/

class FileAssociatedShader : public FileAssociatedAsset
{
public:
    static glow::Shader * getOrCreate(
        GLenum type
    ,   const QString & fileName
    ,   glow::Program & program);


    /** This processes the queue of files with shaders associated by 
        recompiling those and relinking related programs.
        Note: This requires context to be made current!
    */
    static QList<glow::Program *> process();

protected slots:
    void shaderDestroyed(QObject * object);
    void programDestroyed(QObject * object);

private:
    FileAssociatedShader();
    ~FileAssociatedShader();

protected:
    static QMap<QString, glow::Shader *> s_shaderByFilePath;
    static QMultiMap<glow::Shader *, glow::Program *> s_programsByShader;
};
