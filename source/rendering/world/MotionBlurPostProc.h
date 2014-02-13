#pragma once

#include <glow/Program.h>

#include "SimplePostProcPass.h"

class MotionBlurPostProc : public AbstractPostProc
{
public:
    MotionBlurPostProc();
    ~MotionBlurPostProc();

    virtual void apply();
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, glow::Texture*> input);
    virtual glow::Texture* outputTexture();

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
    
    glow::ref_ptr<glow::Texture> m_TMVerticalOutputTexture;
    glow::ref_ptr<glow::Texture> m_TMOutputTexture;
    glow::ref_ptr<glow::Texture> m_NMOutputTexture;
    glow::ref_ptr<glow::Texture> m_randomTexture;
};

template<typename T>
void MotionBlurPostProc::setUniform(const QString name, const T& value) {
    m_blurPass.setUniform(name, value);
}
