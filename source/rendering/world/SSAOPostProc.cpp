#include "SSAOPostProc.h"

#include <glm/gtx/random.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>

#include "Util.h"

const int SSAOPostProc::kernelSize = 32;
const int SSAOPostProc::noiseSize = 4;
const float SSAOPostProc::radius = 25.0f;
const QList<QString> SSAOPostProc::requiredSamplers = { "normal", "depth", "color" };

SSAOPostProc::SSAOPostProc(TextureFormat outputFormat)
:   m_ssaoPass({ GL_RGBA32F, GL_RGBA, GL_FLOAT }, "data/shaders/ssao.frag", "data/shaders/ssao.vert")
,   m_blurPass(outputFormat, "data/shaders/blur.frag")
{
    initialize();
}

SSAOPostProc::~SSAOPostProc()
{
}

void SSAOPostProc::apply()
{
    // first SSAO Pass
    m_ssaoPass.apply();
    
    //second SSAO Pass (blur)
    m_blurPass.apply();
}

void SSAOPostProc::setInputTextures(const QMap<QString, glow::Texture*> & input)
{
    for (QString sampler : requiredSamplers)
        assert(input.contains(sampler));

    //split into 2 Maps for each pass (ssao, blur)
    QMap<QString, glow::Texture*> ssaoInputTextures;
    ssaoInputTextures["normal"] = input.value("normal");
    ssaoInputTextures["depth"] = input.value("depth");
    ssaoInputTextures["noise"] = m_noiseTexture;

    m_ssaoPass.setInputTextures(ssaoInputTextures);

    QMap<QString, glow::Texture*> blurInputTextures;
    blurInputTextures["color"] = input.value("color");
    blurInputTextures["ssao"] = m_ssaoOutputTexture;

    m_blurPass.setInputTextures(blurInputTextures);
}

glow::Texture* SSAOPostProc::outputTexture()
{
    return m_blurPass.outputTexture();
}

void SSAOPostProc::initialize()
{
    m_ssaoOutputTexture = m_ssaoPass.outputTexture();
    m_noiseTexture = Util::create2DTexture();

    //init noise texture and ssao kernel
    glow::Array<glm::vec3> kernel = glow::Array<glm::vec3>();
    for (int i = 0; i < kernelSize; ++i) {
        kernel << glm::normalize(glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(0.1f, 1.0f))
            );

        float scale = glm::linearRand(0.0f, 1.0f);
        scale = glm::mix(0.1f, 1.0f, scale * scale);
        kernel[i] *= scale;
    }

    const int noiseBufferSize = noiseSize*noiseSize;
    glow::Array<glm::vec3> noise = glow::Array<glm::vec3>();
    for (int i = 0; i < noiseBufferSize; ++i) {
        noise << glm::normalize(glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            0.0f)
            );
    }

    m_noiseTexture->image2D(0, GL_RGB32F, noiseSize, noiseSize, 0, GL_RGB, GL_FLOAT, &noise[0]);

    m_ssaoPass.setUniform("noiseTexSize", noiseSize);
    m_ssaoPass.setUniform("kernelSize", kernelSize);
    m_ssaoPass.setUniform("kernel", kernel);
    m_ssaoPass.setUniform("radius", radius);
}

void SSAOPostProc::resize(int width, int height)
{
    m_ssaoPass.setUniform("viewport", glm::vec2(width, height));

    m_ssaoPass.resize(width, height);
    m_blurPass.resize(width, height);
}
