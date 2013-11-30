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

    void draw(int normalTexture, int depthTexture, const glm::vec3 & cameraPosition, const glm::mat3 & normal, const glm::mat4 & projection, const glm::mat4 & viewProjectionInv, glow::Texture & outTexture);
    void resize(int width, int height);

protected:
	void initialize();

protected:
    static const int m_kernelSize = 16;
    static const int m_noiseSize = 4;

    glow::Program m_ssaoProgram;
    glow::Program m_blurProgram;
    glow::ref_ptr<glow::ScreenAlignedQuad> m_ssaoQuad;
    glow::ref_ptr<glow::ScreenAlignedQuad> m_blurQuad;
    glow::ref_ptr<glow::Texture> m_ssaoTexture;
    glow::ref_ptr<glow::Texture> m_noiseTexture;
    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
};
