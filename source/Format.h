
#pragma once

#include <QSurfaceFormat>
#include <QSize>
#include <QMap>

class Format
{
public:
	Format();
	virtual ~Format();

	// similar to QSurfaceFormat and QGLFormat

	// 24 by default
	int	depthBufferSize() const;
	void setDepthBufferSize(const int size);

	int	redBufferSize() const;
	void setRedBufferSize(const int size);

	int	greenBufferSize() const;
	void setGreenBufferSize(const int size);

	int	blueBufferSize() const;
	void setBlueBufferSize(const int size);

	// disabled by default
	int	alphaBufferSize() const;
	void setAlphaBufferSize(const int size);

	// disabled by default
	int	samples() const;
	void setSamples(int samples);

	// only valid version pairs are allowed, on invalid pairs, 
	// nearest major and minor are set.
	void setVersion(
		const unsigned int major
	,	const unsigned int minor);

	unsigned int majorVersion() const; 
	unsigned int minorVersion() const;

	QSurfaceFormat::OpenGLContextProfile profile() const;
	void setProfile(const QSurfaceFormat::OpenGLContextProfile profile);

	// disabled by default
	int	stencilBufferSize() const;
	void setStencilBufferSize(const int size);

	// disabled by default
	bool stereo() const;
	void setStereo(const bool enable);

	// default: QSurfaceFormat::DoubleBuffer
	QSurfaceFormat::SwapBehavior swapBehavior() const;
	void setSwapBehavior(const QSurfaceFormat::SwapBehavior behavior);

protected:
	typedef QMap<unsigned int, unsigned int> MinorsByMajors;
	static const MinorsByMajors validVersions();

protected:
	
	unsigned int m_majorVersion;
	unsigned int m_minorVersion;

	static const MinorsByMajors m_validVersions;

	QSurfaceFormat::OpenGLContextProfile m_profile;

	unsigned int  m_redBufferSize;
	unsigned int  m_greenBufferSize;
	unsigned int  m_blueBufferSize;
	unsigned int  m_alphaBufferSize;

	unsigned int  m_depthBufferSize;
	unsigned int  m_stencilBufferSize;

	unsigned int m_samples;
	bool m_stereo;

	QSurfaceFormat::SwapBehavior m_swapBehavior;
};
