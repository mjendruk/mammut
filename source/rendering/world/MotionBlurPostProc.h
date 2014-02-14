#pragma once

#include "SimplePostProcPass.h"

class MotionBlurPostProc : public AbstractPostProc
{
public:
    MotionBlurPostProc(TextureFormat outputFormat);
    ~MotionBlurPostProc();

    virtual void apply();
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, glow::Texture*> & input);
    virtual glow::Texture* outputTexture();

    void setFPSUniform(float current_targetFPS);

protected:
	void initialize();

protected:
    static const float s_radius;
    static const int s_numSamples;
    static const int s_randomBufferSize;
    static const QList<QString> s_requiredSamplers;

    SimplePostProcPass m_tmVerticalPass; //tm = tileMax
    SimplePostProcPass m_tmHorizontalPass;
    SimplePostProcPass m_neighborMaxPass;
    SimplePostProcPass m_blurPass;
    
    glow::ref_ptr<glow::Texture> m_TMVerticalOutputTexture;
    glow::ref_ptr<glow::Texture> m_TMOutputTexture;
    glow::ref_ptr<glow::Texture> m_NMOutputTexture;
    glow::ref_ptr<glow::Texture> m_randomTexture;
};