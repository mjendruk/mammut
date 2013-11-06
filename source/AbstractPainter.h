
#pragma once

#include <QString>

class AbstractContext;
class Format;


class AbstractPainter
{
public:
    AbstractPainter();
    virtual ~AbstractPainter(); // deletes m_context

    virtual AbstractContext * context() const = 0;

    // return list of mandatory extensions (verify results in fatal if one is not supported)
    virtual const QStringList extensions() const;

    // this should setup a context in m_context with the format provided
    virtual const bool initialize(const Format & format) = 0;
    // returns wether m_context is nullptr or not
    const bool isValid() const;

    virtual void show();
    virtual void hide();

    virtual void paint(const unsigned int targetFBO) = 0;
    virtual void resize(
        const int width
    ,   const int height) = 0;

	const bool verify() const;

protected:
   // uses the context to verify existence/support of each mandatory extension
    const bool verifyExtensions() const;
};