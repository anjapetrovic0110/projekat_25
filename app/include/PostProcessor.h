//
// Created by lenovo on 2. 5. 2026..
//

#ifndef MATF_RG_PROJECT_POSTPROCESSOR_H
#define MATF_RG_PROJECT_POSTPROCESSOR_H
#include "../../engine/libs/glad/include/glad/glad.h"
#include "engine/resources/Shader.hpp"

namespace app {

class PostProcessor {
public:
    void init(int width, int height);
    void render(GLuint hdrTexture);
    void bind_plain();
    void unbind_plain();
    GLuint getPlainTexture() { return plainTexture; }

private:
    GLuint quadVAO;
    engine::resources::Shader *shader;
    int screenWidth;
    int screenHeight;

    GLuint plainFBO;
    GLuint plainTexture;
    void create_plainFBO();
};

}// namespace app

#endif//MATF_RG_PROJECT_POSTPROCESSOR_H
