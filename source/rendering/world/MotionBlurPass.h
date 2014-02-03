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

class MotionBlurPass : public glow::Referenced
{
public:
    MotionBlurPass();
    ~MotionBlurPass();

    void draw(int colorTexture,
              int depthTexture,
              int ssaoTexture,
              const glm::mat4 & currentViewProjectionInverted,
              const glm::mat4 previousViewProjection,
              int currentFPS,
              glow::Texture & outTexture);

protected:
	void initialize();

protected:
    glow::Program m_motionBlurProgram;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_motionBlurQuad;
    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
};
