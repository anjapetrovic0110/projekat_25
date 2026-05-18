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

private:
    GLuint quadVAO;
    engine::resources::Shader *shader;
    int screenWidth;
    int screenHeight;
};

}// namespace app

#endif//MATF_RG_PROJECT_POSTPROCESSOR_H
