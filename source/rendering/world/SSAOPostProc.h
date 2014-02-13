#pragma once

#include "SimplePostProcPass.h"

class SSAOPostProc : public AbstractPostProc
{
public:
    SSAOPostProc(TextureFormat outputFormat);
    virtual ~SSAOPostProc();

    virtual void apply();
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, glow::Texture*> & input);
    virtual glow::Texture* outputTexture();

    template<typename T>
    void setUniform(const QString name, const T& value);

protected:
    void initialize();

protected:
    static const int kernelSize;
    static const int noiseSize;
    static const float radius;
    static const QList<QString> requiredSamplers;

    SimplePostProcPass m_ssaoPass;
    SimplePostProcPass m_blurPass;

    glow::ref_ptr<glow::Texture> m_ssaoOutputTexture;

    glow::ref_ptr<glow::Texture> m_noiseTexture;
};

template<typename T>
void SSAOPostProc::setUniform(const QString name, const T& value) {
    m_ssaoPass.setUniform(name, value);
}
