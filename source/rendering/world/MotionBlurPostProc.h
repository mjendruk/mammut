#pragma once

#include <glow/ref_ptr.h>
#include <glow/Referenced.h>
#include <glow/Program.h>

#include "PostProcInterface.h"

class SimplePostProcPass;

class MotionBlurPostProc : public PostProcInterface
{
public:
    MotionBlurPostProc();
    ~MotionBlurPostProc();

    virtual void apply(glow::FrameBufferObject & fbo);
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, int> & input);
    virtual void set2DTextureOutput(const QMap<GLenum, glow::Texture*> & output);

    template<typename T>
    void setUniform(const QString name, const T& value);

protected:
	void initialize();

protected:
    static const float radius;
    static const float numSamples;

    glow::ref_ptr<SimplePostProcPass> m_tmVerticalPass; //tm = tileMax
    glow::ref_ptr<SimplePostProcPass> m_tmHorizontalPass;
    glow::ref_ptr<SimplePostProcPass> m_neighborMaxPass;
    glow::ref_ptr<SimplePostProcPass> m_blurPass;

    glow::ref_ptr<glow::FrameBufferObject> m_TMTempFBO;
    glow::ref_ptr<glow::FrameBufferObject> m_TMFBO;
    glow::ref_ptr<glow::FrameBufferObject> m_NMFBO;
    
    glow::ref_ptr<glow::Texture> m_TMTempTexture;
    glow::ref_ptr<glow::Texture> m_TMTexture;
    glow::ref_ptr<glow::Texture> m_NMTexture;
    glow::ref_ptr<glow::Texture> m_randomTexture;
};

template<typename T>
void MotionBlurPostProc::setUniform(const QString name, const T& value) {
    m_blurPass->setUniform(name, value);
}
