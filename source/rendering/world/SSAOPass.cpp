#include "SSAOPass.h"

#include <array>

#include <glm/gtx/random.hpp>

#include <glow/Texture.h>
#include <glowutils/Icosahedron.h>

#include "Util.h"

const int SSAOPass::s_kernelSize = 24;
const int SSAOPass::s_noiseSize = 4;
const QList<QString> SSAOPass::s_requiredSamplers = { "normal_depth", "color" };

SSAOPass::SSAOPass()
:   m_ssaoPass("data/shaders/ssao.vert", "data/shaders/ssao.frag", GL_R32F)
,   m_horizontalBlurPass("data/shaders/horizontal_blur.frag", GL_R32F)
,   m_verticalBlurPass("data/shaders/vertical_blur.frag", GL_RGBA32F)
{
    initialize();
}

SSAOPass::~SSAOPass()
{
}

/** This method returns at least minN uniformly distributed vectors located on a z-oriented hemisphere */
void SSAOPass::pointsOnSphere(const unsigned int minN, std::vector<glm::vec3> & points)
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


void SSAOPass::apply()
{
    // first SSAO Pass
    m_ssaoPass.apply();
    
    //second SSAO Pass (blur)
    m_horizontalBlurPass.apply();
    m_verticalBlurPass.apply();
}

void SSAOPass::setInputTextures(const QMap<QString, glow::Texture*> & input)
{
    for (QString sampler : s_requiredSamplers)
        assert(input.contains(sampler));

    //split into 2 Maps for each pass (ssao, blur)
    QMap<QString, glow::Texture *> ssaoInputTextures;
    ssaoInputTextures["normal_depth"] = input.value("normal_depth");
    ssaoInputTextures["noise"] = m_noiseTexture;

    m_ssaoPass.setInputTextures(ssaoInputTextures);

    QMap<QString, glow::Texture *> horizontalBlurInputTextures;
    horizontalBlurInputTextures["ssao"] = m_ssaoTexture;
    horizontalBlurInputTextures["normal_depth"] = input.value("normal_depth");
    m_horizontalBlurPass.setInputTextures(horizontalBlurInputTextures);
    
    QMap<QString, glow::Texture *> verticalBlurInputTextures;
    verticalBlurInputTextures["horizontalBlur"] = m_horizontalBlurTexture;
    verticalBlurInputTextures["normal_depth"] = input.value("normal_depth");
    verticalBlurInputTextures["color"] = input.value("color");
    m_verticalBlurPass.setInputTextures(verticalBlurInputTextures);
}

glow::Texture* SSAOPass::outputTexture()
{
    return m_verticalBlurPass.outputTexture();
}

void SSAOPass::initialize()
{
    m_ssaoTexture = m_ssaoPass.outputTexture();
    m_horizontalBlurTexture = m_horizontalBlurPass.outputTexture();
    m_verticalBlurTexture = m_verticalBlurPass.outputTexture();
    
    m_noiseTexture = Util::create2DTexture();

    //init noise texture and ssao kernel
    std::vector<glm::vec3> kernel;
    
    for (int i = 0; i < s_kernelSize; ++i) {
        glm::vec3 sample = glm::vec3(glm::linearRand(-1.0f, 1.0f),
                                     glm::linearRand(-1.0f, 1.0f),
                                     glm::linearRand(0.0f, 1.0f));
        
        sample *= glm::linearRand(0.0f, 1.0f);
        
        kernel.push_back(sample);
    }
    
//    std::vector<glm::vec3> kernel = std::vector<glm::vec3>();
//    std::vector<glm::vec3> hemisphere;
//    pointsOnSphere(s_kernelSize, hemisphere);
//    
//    for (int i = 0; i < s_kernelSize; ++i) {
//        kernel.push_back(hemisphere[i]);
//        
//        float scale = glm::linearRand(0.0f, 1.0f);
//        scale = glm::mix(0.1f, 1.0f, scale * scale);
//        kernel[i] *= scale;
//    }
    

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
    m_ssaoPass.setUniform("kernel", kernel);
}

void SSAOPass::resize(int width, int height)
{
    m_ssaoPass.setUniform("viewport", glm::vec2(width, height));

    m_ssaoPass.resize(width, height);
    m_horizontalBlurPass.resize(width, height);
    m_verticalBlurPass.resize(width, height);
}

void SSAOPass::setProjectionUniform(const glm::mat4 & projection)
{
    m_ssaoPass.setUniform("projection", projection);
}

void SSAOPass::setInverseProjectionUniform(const glm::mat4 & invProjection)
{
    m_ssaoPass.setUniform("invProjection", invProjection);
}

void SSAOPass::setNormalMatrixUniform(const glm::mat3 & normalMatrix)
{
    m_ssaoPass.setUniform("normalMatrix", normalMatrix);
}

void SSAOPass::setFarPlaneUniform(float farPlane)
{
    m_ssaoPass.setUniform("farPlane", farPlane);
}
