
#include <cassert>

#include "Format.h"

const Format::MinorsByMajors Format::m_validVersions
	= Format::validVersions();


Format::Format()
:	m_majorVersion(3)
,	m_minorVersion(2)

,	m_profile(QSurfaceFormat::CoreProfile)

,	m_redBufferSize(8)
,	m_greenBufferSize(8)
,	m_blueBufferSize(8)
,	m_alphaBufferSize(8)

,	m_depthBufferSize(24)
,	m_stencilBufferSize(0)

,	m_stereo(false)
,	m_swapBehavior(QSurfaceFormat::DoubleBuffer)

,	m_samples(0)
{
}

Format::~Format()
{
}

const Format::MinorsByMajors Format::validVersions()
{
	MinorsByMajors minorsByMajors;

	minorsByMajors.insertMulti(1, 0);
	minorsByMajors.insertMulti(1, 1);
	minorsByMajors.insertMulti(1, 2);
	minorsByMajors.insertMulti(1, 3);
	minorsByMajors.insertMulti(1, 4);
	minorsByMajors.insertMulti(1, 5);

	minorsByMajors.insertMulti(2, 0);
	minorsByMajors.insertMulti(2, 1);

	minorsByMajors.insertMulti(3, 0);
	minorsByMajors.insertMulti(3, 1);
	minorsByMajors.insertMulti(3, 2);
	minorsByMajors.insertMulti(3, 3);

	minorsByMajors.insertMulti(4, 0);
	minorsByMajors.insertMulti(4, 1);
	minorsByMajors.insertMulti(4, 2);
	minorsByMajors.insertMulti(4, 3);

	return minorsByMajors;
}

void Format::setVersion(
	const unsigned int major
,	const unsigned int minor)
{
	typedef QList<unsigned int> t_minors;

	// retrieve nearest valid major
	m_majorVersion = m_validVersions.lowerBound(major).key();
	const t_minors minors(m_validVersions.values(m_majorVersion));

	assert(!minors.isEmpty());

	if (major == m_majorVersion)
	{
		// get nearest valid minor
		m_minorVersion = 0;
		foreach(unsigned int m, minors)
			if (m_minorVersion <= m && m <= minor)
				m_minorVersion = m;
	}
	else
		// get highest valid minor
		m_minorVersion = minors.back();

	if (minor != m_minorVersion
		|| major != m_majorVersion)
		qWarning("Unknown OpenGL Version %i.%i was adjusted to %i.%i."
		, major, minor, m_majorVersion, m_minorVersion);
}

unsigned int Format::majorVersion() const
{
	return m_majorVersion;
}

unsigned int Format::minorVersion() const
{
	return m_minorVersion;
}

QSurfaceFormat::OpenGLContextProfile Format::profile() const
{
	return m_profile;
}

void Format::setProfile(const QSurfaceFormat::OpenGLContextProfile profile)
{
	m_profile = profile;
}

int Format::redBufferSize() const
{
	return m_redBufferSize;
}

void Format::setRedBufferSize(const int size)
{
	m_redBufferSize = size;
}

int Format::greenBufferSize() const
{
	return m_greenBufferSize;
}

void Format::setGreenBufferSize(const int size)
{
	m_greenBufferSize = size;
}

int  Format::blueBufferSize() const
{
	return m_blueBufferSize;
}

void Format::setBlueBufferSize(const int size)
{
	m_blueBufferSize = size;
}

int Format::alphaBufferSize() const
{
	return m_alphaBufferSize;
}

void Format::setAlphaBufferSize(const int size)
{
	m_alphaBufferSize = size;
}

int Format::depthBufferSize() const
{
	return m_depthBufferSize;
}

void Format::setDepthBufferSize(const int size)
{
	m_depthBufferSize = size;
}

int Format::stencilBufferSize() const
{
	return m_stencilBufferSize;
}

void Format::setStencilBufferSize(const int size)
{
	m_stencilBufferSize = size;
}

QSurfaceFormat::SwapBehavior Format::swapBehavior() const
{
	return m_swapBehavior;
}

void Format::setSwapBehavior(const QSurfaceFormat::SwapBehavior behavior)
{
	m_swapBehavior = behavior;
}

bool Format::stereo() const
{
	return m_stereo;
}

void Format::setStereo(const bool enable)
{
	m_stereo = enable;
}

int Format::samples() const
{
	return m_samples;
}

void Format::setSamples(const int samples)
{
	m_samples = samples;
}
