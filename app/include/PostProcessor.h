
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
    GLuint quadVAO = 0;
    GLuint quadVBO = 0;
    engine::resources::Shader *shader;
    int screenWidth = 0;
    int screenHeight = 0;

    GLuint plainDepth = 0;
    GLuint plainFBO = 0;
    GLuint plainTexture = 0;
    void create_plainFBO();
};

}// namespace app

#endif//MATF_RG_PROJECT_POSTPROCESSOR_H
