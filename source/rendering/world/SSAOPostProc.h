#pragma once

#include <glow/Program.h>

#include "PostProcInterface.h"

class SimplePostProcPass;

class SSAOPostProc : public PostProcInterface
{
public:
    SSAOPostProc();
    virtual ~SSAOPostProc();

    virtual void apply(glow::FrameBufferObject & frameBuffer);
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, int> & input);
    virtual void set2DTextureOutput(const QMap<GLenum, glow::Texture*> & output);

    template<typename T>
    void setUniform(const QString name, const T& value);

protected:
    void initialize();
    void initializeSSAOPrograms();

protected:
    static const int m_kernelSize;
    static const int m_noiseSize;
    static const float m_radius;

    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_ssaoQuad;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_blurQuad;

    glow::ref_ptr<glow::Texture> m_ssaoTexture;
    glow::ref_ptr<glow::Texture> m_noiseTexture;
    glow::ref_ptr<glow::FrameBufferObject> m_tempFBO;

    glow::ref_ptr<SimplePostProcPass> m_ssaoPass;
    glow::ref_ptr<SimplePostProcPass> m_blurPass;
};

template<typename T>
void SSAOPostProc::setUniform(const QString name, const T& value) {
    m_ssaoPass->setUniform(name, value);
}
