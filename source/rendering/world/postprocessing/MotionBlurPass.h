#pragma once

#include <GL/glew.h>

#include "SimplePostProcPass.h"


class MotionBlurPass : public AbstractPostProcPass
{
public:
    MotionBlurPass();
    ~MotionBlurPass();

    void apply() override;
    void resize(int width, int height) override;
    
    void setInputTextures(const QMap<QString, glow::Texture*> & input) override;
    glow::Texture * outputTexture() override;

    void setFPSUniform(float currentTargetFPS);

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
