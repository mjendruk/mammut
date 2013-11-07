
#include <cassert>
#include <QFileSystemWatcher>
#include <QTextStream>
#include <QFileInfo>
#include "FileAssociatedShader.h"

QMap<QString, QOpenGLShader *> FileAssociatedShader::s_shaderByFilePath;
QMultiMap<QOpenGLShader *, QOpenGLShaderProgram *> FileAssociatedShader::s_programsByShader;

FileAssociatedShader::FileAssociatedShader()
{}

FileAssociatedShader::~FileAssociatedShader()
{
    s_shaderByFilePath.clear();
    s_programsByShader.clear();
}

QOpenGLShader * FileAssociatedShader::getOrCreate(
    QOpenGLShader::ShaderType type
,   const QString & fileName
,   QOpenGLShaderProgram & program)
{
    QOpenGLShader * shader(nullptr);
	
	QFileInfo fi(fileName);
	if (!fi.exists())
	{
		qWarning() << fileName << " does not exist: shader is without source and associated file.";
		return shader;
	}

	QString filePath(fi.absoluteFilePath());
    if (s_shaderByFilePath.contains(filePath))
    {
        shader = s_shaderByFilePath[filePath];
    }
    else
    {
		instance()->addResourcePath(filePath);

        shader = new QOpenGLShader(type);
        shader->compileSourceFile(filePath);

        connect(
			shader,
			&QOpenGLShader::destroyed,
			static_cast<FileAssociatedShader*>(instance()),
			&FileAssociatedShader::shaderDestroyed);

        s_shaderByFilePath.insert(filePath, shader);
    }

    if (!s_programsByShader.contains(shader, &program))
    {
        connect(
			&program,
			&QOpenGLShaderProgram::destroyed,
			static_cast<FileAssociatedShader*>(instance()),
			&FileAssociatedShader::programDestroyed);

        s_programsByShader.insert(shader, &program);

        program.addShader(shader);
    }
    return shader;
}

void FileAssociatedShader::shaderDestroyed(QObject * object)
{
    QOpenGLShader * shader = static_cast<QOpenGLShader*>(object);

    const QString filePath(s_shaderByFilePath.key(shader));
    assert(!filePath.isEmpty());

    fileSystemWatcher()->removePath(filePath);
    s_shaderByFilePath.remove(filePath);

    const auto affectedPrograms = s_programsByShader.values(shader);
    s_programsByShader.remove(shader);

    // disconnect from all programs that have no file associated shaders anymore...
    for (QOpenGLShaderProgram * program : affectedPrograms)
        if (s_programsByShader.keys(program).isEmpty())
            disconnect(
				program,
				&QOpenGLShaderProgram::destroyed,
				static_cast<FileAssociatedShader*>(instance()),
				&FileAssociatedShader::programDestroyed);
}

void FileAssociatedShader::programDestroyed(QObject * object)
{
    QOpenGLShaderProgram * program = static_cast<QOpenGLShaderProgram*>(object);

    const auto affectedShaders = s_programsByShader.keys(program);

    for (QOpenGLShader * shader : affectedShaders)
        s_programsByShader.remove(shader, program);

    for (QOpenGLShader * shader : affectedShaders)
        if (s_programsByShader.values(shader).isEmpty())
        {
            const QString filePath(s_shaderByFilePath.key(shader));
            assert(!filePath.isEmpty());

            fileSystemWatcher()->removePath(filePath);
            s_shaderByFilePath.remove(filePath);

			disconnect(shader,
				&QOpenGLShader::destroyed,
				static_cast<FileAssociatedShader*>(instance()),
				&FileAssociatedShader::shaderDestroyed);
        }
}

QList<QOpenGLShaderProgram *> FileAssociatedShader::process()
{
    QList<QOpenGLShaderProgram *> programsWithInvalidatedUniforms;

    while (!s_queue.isEmpty())
    {
        QString filePath = s_queue.first();
        s_queue.removeFirst();

        QOpenGLShader * shader(s_shaderByFilePath[filePath]);
        assert(shader != nullptr);

        qDebug() << "Recompiling" << filePath;

        if (shader->isCompiled())
        {
            const QByteArray backup(shader->sourceCode());

            // if current version works, use its source code as
            // backup if new changes lead to uncompilable shader.
            if (!shader->compileSourceFile(filePath))
                shader->compileSourceCode(backup);
        }
        else
            shader->compileSourceFile(filePath);

        auto programs(s_programsByShader.values(shader));
        assert(!programs.isEmpty());

        programsWithInvalidatedUniforms << programs;

        for (QOpenGLShaderProgram * program : programs)
            if (!program->link())
                qWarning() << program->log();         
    }

    return programsWithInvalidatedUniforms;
}
