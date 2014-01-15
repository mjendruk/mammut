#pragma once

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void initialize();
    virtual void resize(int width, int height) = 0;
    virtual void render(float devicePixelRatio) = 0;

    bool initialized() const;

protected:
    bool m_initialized;
};
