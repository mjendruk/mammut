#pragma once

#include <glow/ref_ptr.h>
#include <glow/Referenced.h>
#include <glow/Program.h>

namespace glow
{
    class Texture;
    class FrameBufferObject;
}

namespace glowutils
{
    class ScreenAlignedQuad;
}

class SSAO : public glow::Referenced
{
public:
    SSAO();
    ~SSAO();

    void draw(int normalTexture, int depthTexture, const glm::mat3 & normal, const glm::mat4 & projection, glow::Texture & outTexture);
    void resize(int width, int height);

protected:
	void initialize();

protected:
    static const int m_kernelSize;
    static const int m_noiseSize;

    glow::Program m_ssaoProgram;
    glow::Program m_blurProgram;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_ssaoQuad;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_blurQuad;
    glow::ref_ptr<glow::Texture> m_ssaoTexture;
    glow::ref_ptr<glow::Texture> m_noiseTexture;
    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
};
