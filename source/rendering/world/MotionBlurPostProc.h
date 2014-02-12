#pragma once

#include <glow/Program.h>

#include "SimplePostProcPass.h"

class MotionBlurPostProc : public AbstractPostProc
{
public:
    MotionBlurPostProc();
    ~MotionBlurPostProc();

    virtual void apply(glow::FrameBufferObject & fbo);
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, int> input);

    template<typename T>
    void setUniform(const QString name, const T& value);

protected:
	void initialize();

protected:
    static const float radius;
    static const int numSamples;

    SimplePostProcPass m_tmVerticalPass; //tm = tileMax
    SimplePostProcPass m_tmHorizontalPass;
    SimplePostProcPass m_neighborMaxPass;
    SimplePostProcPass m_blurPass;

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
    m_blurPass.setUniform(name, value);
}
