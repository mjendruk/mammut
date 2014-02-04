#pragma once

#include <glow/ref_ptr.h>
#include <glow/Referenced.h>
#include <glow/Program.h>

#include "PostprocessingPass.h"

namespace glow
{
    class Texture;
    class FrameBufferObject;
}

namespace glowutils
{
    class ScreenAlignedQuad;
}

class MotionBlurPass : public PostprocessingPass
{
public:
    MotionBlurPass(const QString name);
    ~MotionBlurPass();

    virtual void apply(glow::FrameBufferObject & fbo);
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, int> & input);

protected:
	void initialize();
    void initializePrograms();

protected:
    static const float radius;
    static const float numSamples;

    glow::ref_ptr<glow::Program> m_TMProgram;
    glow::ref_ptr<glow::Program> m_NMProgram;
    glow::ref_ptr<glow::Program> m_blurProgram;

    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_TMQuad;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_NMQuad;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_blurQuad;

    glow::ref_ptr<glow::FrameBufferObject> m_TMTempFBO;
    glow::ref_ptr<glow::FrameBufferObject> m_TMFBO;
    glow::ref_ptr<glow::FrameBufferObject> m_NMFBO;
    
    glow::ref_ptr<glow::Texture> m_TMTempTexture;
    glow::ref_ptr<glow::Texture> m_TMTexture;
    glow::ref_ptr<glow::Texture> m_NMTexture;
    glow::ref_ptr<glow::Texture> m_randomTexture;

    QMap<QString, int> * m_TMTempInputTextures;
    QMap<QString, int> * m_TMInputTextures;
    QMap<QString, int> * m_NMInputTextures;
    QMap<QString, int> * m_blurInputTextures;
};
