#pragma once

#include "SimplePostProcPass.h"

class SSAOPostProc : public AbstractPostProc
{
public:
    SSAOPostProc();
    virtual ~SSAOPostProc();

    virtual void apply(glow::FrameBufferObject & frameBuffer);
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, int> input);

    template<typename T>
    void setUniform(const QString name, const T& value);

protected:
    void initialize();

protected:
    static const int m_kernelSize;
    static const int m_noiseSize;
    static const float m_radius;

    SimplePostProcPass m_ssaoPass;
    SimplePostProcPass m_blurPass;

    glow::ref_ptr<glow::Texture> m_ssaoTexture;
    glow::ref_ptr<glow::Texture> m_noiseTexture;

    glow::ref_ptr<glow::FrameBufferObject> m_tempFBO;
};

template<typename T>
void SSAOPostProc::setUniform(const QString name, const T& value) {
    m_ssaoPass.setUniform(name, value);
}
