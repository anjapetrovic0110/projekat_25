//
// Created by lenovo on 2. 5. 2026..
//

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
    int width;
    int height;
    int samples;

    GLuint msaaFBO;
    GLuint colorBuffer;
    GLuint depthBuffer;

    GLuint resolveFBO;
    GLuint resolvedTexture;
};

}// namespace app

#endif//MATF_RG_PROJECT_RENDERTARGET_H
