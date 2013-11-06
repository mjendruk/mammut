
#include <QStringList> // error qmetatype.h must be included before any header file that defines Bool

#include <cassert>

#ifdef WIN32
    #include <windows.h>
    #include <gl/GL.h>
    #include <gl/GLU.h>
#else
    #include <GL/glx.h>
    #include <GL/glu.h>
#endif

#include "AbstractContext.h"

const AbstractContext::ProfileIdentifier AbstractContext::m_validProfileIdentifier 
    = AbstractContext::validProfileIdentifier();

const AbstractContext::SwapbIdentifier AbstractContext::m_swapbIdentifier 
    = AbstractContext::validSwapBehaviorIdentifier();


AbstractContext::AbstractContext()
:	m_swapInterval(-1)
{
}

AbstractContext::~AbstractContext()
{
}

bool AbstractContext::isValid() const
{
	return 0 < id();
}

bool AbstractContext::hasExtension(const QByteArray & extension) const
{
	return extensions().contains(extension);
}

const Format & AbstractContext::format() const
{
	return m_format;
}

int AbstractContext::tryFetchId()
{
	assert(isValid());
	if (!isValid())
		return 0;

	makeCurrent();
	glError();

#ifdef WIN32
	const HGLRC handle = wglGetCurrentContext();
	const int id = reinterpret_cast<int>(handle);
#elsif LINUX
	const GLXContext handle = glXGetCurrentContext();
	const int id = static_cast<int>(reinterpret_cast<long long>(handle));
#else
    const int id = 0;
#endif

	if (0 == id)
		qCritical("Acquiring OpenGL context handle failed.");

	doneCurrent();
	glError();

	return id;
}

int AbstractContext::swapInterval() const
{
    return m_swapInterval;
}

bool AbstractContext::setSwapInterval(const int interval)
{
    if(interval == m_swapInterval)
        return true;

    bool result(false);

#ifdef WIN32

    typedef bool (WINAPI * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

    if(!wglSwapIntervalEXT)
        wglSwapIntervalEXT = reinterpret_cast<SWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
    if(wglSwapIntervalEXT)
        result = wglSwapIntervalEXT(interval);

#elif __APPLE__

    qWarning("TODO: Setting swap interval is currently not implemented for __APPLE__");

#else

    typedef int (APIENTRY * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC glXSwapIntervalSGI = nullptr;

    if(!glXSwapIntervalSGI)
    {
        const GLubyte * sgi = reinterpret_cast<const GLubyte*>("glXSwapIntervalSGI");
        glXSwapIntervalSGI = reinterpret_cast<SWAPINTERVALEXTPROC>(glXGetProcAddress(sgi));
    }
    if(glXSwapIntervalSGI)
        result = glXSwapIntervalSGI(interval);

#endif

    if(!result)
    {
        // Note: could probably be replaced by QOpenGLDebugLogger?
        glError();
        qWarning("Setting swap interval to %i failed.", interval);
    }
    else
        m_swapInterval = interval;

    return result;
}

bool AbstractContext::verticalSync() const
{
    return m_swapInterval > 0;
}

bool AbstractContext::setVerticalSync(const bool enable)
{
    return setSwapInterval(enable ? 1 : 0);
}

AbstractContext::ProfileIdentifier AbstractContext::validProfileIdentifier()
{
    ProfileIdentifier profileIdentifier;

    profileIdentifier[QSurfaceFormat::NoProfile]            = "NoProfile";
    profileIdentifier[QSurfaceFormat::CoreProfile]          = "CoreProfile";
    profileIdentifier[QSurfaceFormat::CompatibilityProfile] = "CompatibilityProfile";

    return profileIdentifier;
}

AbstractContext::SwapbIdentifier AbstractContext::validSwapBehaviorIdentifier()
{
    SwapbIdentifier swapbIdentifier;

    swapbIdentifier[QSurfaceFormat::DefaultSwapBehavior]    = "DefaultSwapBehavior";
    swapbIdentifier[QSurfaceFormat::SingleBuffer]           = "SingleBuffer";
    swapbIdentifier[QSurfaceFormat::DoubleBuffer]           = "DoubleBuffer";
    swapbIdentifier[QSurfaceFormat::TripleBuffer]           = "TripleBuffer";

    return swapbIdentifier;
}


// VERIFICATION

bool AbstractContext::verify(
	const Format & requested
,	const Format & created)
{
    bool result = true;

//    const Format & current = format();

    result &= verifyVersionAndProfile(requested, created);
	result &= verifyPixelFormat(requested, created);

    return result;
}

bool AbstractContext::verifyVersionAndProfile(
    const Format & requested
,	const Format & current)
{
    const bool sameProfiles(requested.profile() == current.profile());

    if(!sameProfiles)
    {
        qWarning("A context with a different profile as requested was created:");
        const QString info(QString("%1 requested, %2 created.\n")
            .arg(m_validProfileIdentifier[requested.profile()])
            .arg(m_validProfileIdentifier[current.profile()]));
        qWarning("%s", qPrintable(info));
    }

    if(requested.majorVersion() != current.majorVersion() 
    || requested.minorVersion() != current.minorVersion())
    {
        qWarning("A context with a different OpenGL Version as requested was created:");
        const QString info(QString("%1.%2 requested, %3.%4 created.\n")
            .arg(requested.majorVersion()).arg(requested.minorVersion())
            .arg(current.majorVersion()).arg(current.minorVersion()));
        qWarning("%s", qPrintable(info));

        if(requested.profile() == QSurfaceFormat::CoreProfile)
            return false;
    }
    else
    {
        //const QString info(QString("Context Version: %1.%2\n")
        //    .arg(current.majorVersion()).arg(current.minorVersion()));
        //qDebug("%s", qPrintable(info));
    }

    return sameProfiles;
}

inline void AbstractContext::verifyBufferSize(
    const unsigned int sizeRequested
,   const unsigned int sizeInitialized
,   const QString & warning
,   QStringList & issues)
{
    if(sizeRequested == sizeInitialized)
        return;

    issues << (warning + " size mismatch: %1 requested, %2 created.")
        .arg(sizeRequested).arg(sizeInitialized);
}

bool AbstractContext::verifyPixelFormat(
    const Format & requested
,   const Format & current)
{
    QStringList issues;

    const bool sameSwapBehaviors(requested.swapBehavior() == current.swapBehavior());

    if(!sameSwapBehaviors)
    {
        qWarning("A context with a different swap behavior as requested was initialized:");
        const QString info(QString("%1 requested, %2 created.\n")
            .arg(m_swapbIdentifier[requested.swapBehavior()])
            .arg(m_swapbIdentifier[current.swapBehavior()]));
        qWarning("%s", qPrintable(info));
    }

    if(requested.depthBufferSize())
    {
        if(!current.depthBufferSize())
            issues << QString("Depth Buffer requested, but none created.");
        else
            verifyBufferSize(requested.depthBufferSize(), current.depthBufferSize()
                , "Depth Buffer", issues);
    }

    verifyBufferSize(requested.redBufferSize(),   current.redBufferSize()
        , "Red Buffer", issues);
    verifyBufferSize(requested.greenBufferSize(), current.greenBufferSize()
        , "Green Buffer", issues);
    verifyBufferSize(requested.blueBufferSize(),  current.blueBufferSize()
        , "Blue Buffer",  issues); 
    verifyBufferSize(requested.alphaBufferSize(), current.alphaBufferSize()
        , "Alpha Buffer", issues);

    if(requested.stencilBufferSize())
    {
        if(!current.stencilBufferSize())
            issues << QString("Stencil Buffer requested, but none created.");
        else
            verifyBufferSize(requested.stencilBufferSize(), current.stencilBufferSize()
                , "Stencil Buffer", issues);             
    }

    if(requested.stereo() && !current.stereo())
        issues << QString("Stereo Buffering requested, but not initialized.");

    if(requested.samples())
    {
        if(!current.samples())
            issues << QString("Sample Buffers requested, but none initialized.");
        else
            verifyBufferSize(requested.samples(), current.samples()
                , "Samples ", issues);             
    }

    if(issues.isEmpty())
        return true;

    qWarning("Initialized Pixelformat did not match the Requested One:");
    foreach(const QString & issue, issues)
        qWarning("%s", qPrintable(issue));
    qWarning("");

    return false;
}

QSet<QByteArray> & AbstractContext::extensions()
{
	return m_extensions;
}

const QSet<QByteArray> & AbstractContext::extensions() const
{
	return m_extensions;
}

void AbstractContext::setExtensions(const QSet<QByteArray> & extensions)
{
	m_extensions = extensions;
}

bool AbstractContext::error(
    const char * file
,   const int line)
{
    const GLenum error(glGetError());
    const bool errorOccured(GL_NO_ERROR != error);

    if(errorOccured) 
        qCritical("OpenGL: %s [0x%x %s : %i]\n", gluErrorString(error), error, file, line);

    return errorOccured;
}