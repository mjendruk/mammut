
#pragma once

#include <QMap>
#include <QSet>
#include <QByteArray>

#include "Format.h"


class QStringList;

class Surface;

class AbstractContext
{
public:
	// checks for opengl error and on occurence, logs it with
	// source file and line information.
	static bool error(
		const char * file
	,	const int line);

public:
    AbstractContext();
    virtual ~AbstractContext();

	// should return isValid() and set m_format to the surface's format
	virtual bool create(const Format & format) = 0;
	// returns the context id - if this is somehow encapsulated tryFetchId for retrieval.
    virtual int id() const = 0;

	virtual bool makeCurrent() = 0;
    virtual void doneCurrent() = 0;

	typedef QSet<QByteArray> Extensions;
	Extensions & extensions();
	void setExtensions(const Extensions & extensions);
	const Extensions & extensions() const;

    virtual bool hasExtension(const QByteArray & extension) const;

	// this should be in sync to the created context, not the requested one
    virtual const Format & format() const; 

	// returns true if the context was created (irrespective of format verification)
	// and if id() returns id > 0.
	virtual bool isValid() const;

	// swap interval relates to context, since there might be multiple 
	// shared contexts with same format, but individual swap format.
	bool setSwapInterval(const int interval);
	int swapInterval() const;

	// vertical sync is basically accessing swap interval with 0/1
	bool verticalSync() const;
	bool setVerticalSync(const bool enable);


	// checks the created format against the requested
	// Todo: move to protected in future architecture
	static bool verify(
		const Format & requested
		, const Format & created);

protected:

	// compares (logged if erroneous) version and profile between both formats
    static bool verifyVersionAndProfile(
        const Format & requested
    ,   const Format & current);

	// compares (logged if erroneous) buffer sizes and more between both formats
    static bool verifyPixelFormat(
        const Format & requested
    ,   const Format & current);

	// used as inline by verifyPixelFormat
    static void verifyBufferSize(
        const unsigned int sizeRequested
    ,   const unsigned int sizeInitialized
    ,   const QString & warning
    ,   QStringList & issues);

	// operates by making the context current and fetching its context id
	// NOTE: this is not thread safe, so this might result in errorneuos behavior or wrong id.
	int tryFetchId();

protected:
	// these are exclusively used for logging during verification

    typedef QMap<QSurfaceFormat::OpenGLContextProfile, QString> ProfileIdentifier;
    static ProfileIdentifier validProfileIdentifier();

    typedef QMap<QSurfaceFormat::SwapBehavior, QString> SwapbIdentifier;
    static SwapbIdentifier validSwapBehaviorIdentifier();

protected:
    static const ProfileIdentifier m_validProfileIdentifier;
    static const SwapbIdentifier m_swapbIdentifier;

protected:
	int m_swapInterval;
    Format m_format;

	Extensions m_extensions;
};

#define glError() AbstractContext::error(__FILE__, __LINE__)