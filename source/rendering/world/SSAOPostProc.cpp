#include "SSAOPostProc.h"

#include <array>

#include <glm/gtx/random.hpp>

#include <glow/Texture.h>
#include <glowutils/Icosahedron.h>

#include "Util.h"

const int SSAOPostProc::s_kernelSize = 32;
const int SSAOPostProc::s_noiseSize = 5;
const float SSAOPostProc::s_radius = 25.0f;
const QList<QString> SSAOPostProc::s_requiredSamplers = { "normal", "depth", "color" };

std::mt19937 rng;

SSAOPostProc::SSAOPostProc(TextureFormat outputFormat)
:   m_ssaoPass({ GL_RGBA32F, GL_RGBA, GL_FLOAT }, "data/shaders/ssao.frag", "data/shaders/ssao.vert")
,   m_blurPass(outputFormat, "data/shaders/blur.frag")
{
    initialize();
}

SSAOPostProc::~SSAOPostProc()
{
}

/** This method returns at least minN uniformly distributed vectors located on a z-oriented hemisphere */
void SSAOPostProc::pointsOnSphere(const unsigned int minN, std::vector<glm::vec3> & points)
{
    //get vectors uniformly distributed on a sphere
    auto v = glowutils::Icosahedron::vertices();
    auto i = glowutils::Icosahedron::indices();

    std::vector<glm::vec3> vertices(v.begin(), v.end());
    std::vector<glm::lowp_uvec3> indices(i.begin(), i.end());

    int r = static_cast<int>(ceil(log(static_cast<float>(minN * 2 / 12.f)) / log(4.f))); // N = 12 * 4 ^ r
    glowutils::Icosahedron::refine(vertices, indices, r);

    //remove lower hemisphere
    const int size(static_cast<int>(vertices.size()));
    for (int i = 0; i < size; ++i)
    if (vertices[i].z > 0.3f)
        points.push_back(vertices[i]);

    std::shuffle(points.begin(), points.end(), rng);
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
    for (QString sampler : s_requiredSamplers)
        assert(input.contains(sampler));

    //split into 2 Maps for each pass (ssao, blur)
    QMap<QString, glow::Texture*> ssaoInputTextures;
    ssaoInputTextures["normal"] = input.value("normal");
    ssaoInputTextures["depth"] = input.value("depth");
    ssaoInputTextures["noise"] = m_noiseTexture;

    m_ssaoPass.setInputTextures(ssaoInputTextures);

    QMap<QString, glow::Texture*> blurInputTextures;
    blurInputTextures["color"] = input.value("color");
    blurInputTextures["normal"] = input.value("normal");
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
    std::vector<glm::vec3> kernel = std::vector<glm::vec3>();
        std::vector<glm::vec3> hemisphere;
        pointsOnSphere(s_kernelSize, hemisphere);

    for (int i = 0; i < s_kernelSize; ++i) {
        kernel.push_back(hemisphere[i]);

        float scale = glm::linearRand(0.0f, 1.0f);
        scale = glm::mix(0.1f, 1.0f, scale * scale);
        kernel[i] *= scale;
    }

    const int noiseBufferSize = s_noiseSize * s_noiseSize;
    std::vector<glm::vec3> noise = std::vector<glm::vec3>();
    for (int i = 0; i < noiseBufferSize; ++i) {
        noise.push_back(glm::normalize(glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            0.0f)
            ));
    }

    m_noiseTexture->image2D(0, GL_RGB32F, s_noiseSize, s_noiseSize, 0, GL_RGB, GL_FLOAT, &noise[0]);

    m_ssaoPass.setUniform("noiseTexSize", s_noiseSize);
    m_ssaoPass.setUniform("kernelSize", s_kernelSize);
    m_ssaoPass.setUniform("radius", s_radius);
    m_ssaoPass.setUniform("kernel", kernel);
}

void SSAOPostProc::resize(int width, int height)
{
    m_ssaoPass.setUniform("viewport", glm::vec2(width, height));

    m_ssaoPass.resize(width, height);
    m_blurPass.resize(width, height);
}

void SSAOPostProc::setProjectionUniform(glm::mat4 projection)
{
    m_ssaoPass.setUniform("projection", projection);
}

void SSAOPostProc::setInverseProjectionUniform(glm::mat4 invProjection)
{
    m_ssaoPass.setUniform("invProjection", invProjection);
}
void SSAOPostProc::setNormalMatrixUniform(glm::mat3 normalMatrix)
{
    m_ssaoPass.setUniform("normalMatrix", normalMatrix);
}
