#pragma once

#include <glow/ref_ptr.h>
#include <glow/Referenced.h>
#include <glow/Program.h>

namespace glow
{
    class ScreenAlignedQuad;
    class Texture;
    class FrameBufferObject;
}

class SSAO : public glow::Referenced
{
public:
    SSAO();
    ~SSAO();

    void draw(int normalTexture, int depthTexture, glow::Texture & outTexture);
    void resize(int width, int height);

protected:
	void initialize();

protected:
    glow::Program m_ssaoProgram;
    glow::Program m_blurProgram;
    glow::ref_ptr<glow::ScreenAlignedQuad> m_ssaoQuad;
    glow::ref_ptr<glow::ScreenAlignedQuad> m_blurQuad;
    glow::ref_ptr<glow::Texture> m_ssaoTexture;
    glow::ref_ptr<glow::FrameBufferObject> m_fbo;

};
