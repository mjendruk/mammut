#pragma once

#include <glow/Program.h>

#include "PostprocessingPass.h"

class SSAOPass : public PostprocessingPass
{
public:
    SSAOPass(QString name);
    virtual ~SSAOPass();

    virtual void apply(glow::FrameBufferObject & frameBuffer);
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, int> & input);

protected:
    void initialize();
    void initializeSSAOPrograms();

protected:
    static const int m_kernelSize;
    static const int m_noiseSize;
    static const float m_radius;

    glow::ref_ptr<glow::Program> m_blurProgram;
    glow::ref_ptr<glow::Program> m_ssaoProgram;
    QMap<QString, int> m_ssaoInputTextures;
    QMap<QString, int> m_blurInputTextures;

    glow::ref_ptr<glow::Texture> m_ssaoTexture;
    glow::ref_ptr<glow::Texture> m_noiseTexture;
    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
};
