
#ifndef MATF_RG_PROJECT_RENDERTARGET_H
#define MATF_RG_PROJECT_RENDERTARGET_H

#include "../../engine/libs/glad/include/glad/glad.h"

namespace app {

class MSAAFramebuffer {
public:
    void init(int width, int height, int samples);
    void bind();
    void unbind();
    void resolve();
    GLuint getTexture() const { return resolvedTexture; }

private:
    int width = 0;
    int height = 0;
    int samples = 0;

    GLuint msaaFBO = 0;
    GLuint colorBuffer = 0;
    GLuint depthBuffer = 0;

    GLuint resolveFBO = 0;
    GLuint resolvedTexture = 0;
};

}// namespace app

#endif//MATF_RG_PROJECT_RENDERTARGET_H
