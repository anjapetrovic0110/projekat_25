
#ifndef MATF_RG_PROJECT_POINTSHADOWMAP_H
#define MATF_RG_PROJECT_POINTSHADOWMAP_H


#include "../../engine/libs/glad/include/glad/glad.h"
namespace app {

class PointShadowMap {

public:
    GLuint depthCubemap = 0;
    GLuint depthMapFBO = 0;
    const unsigned int SHADOW_WIDTH = 1024;
    const unsigned int SHADOW_HEIGHT = 1024;

    void init();
    void bind();
    void unbind();
};

}// namespace app

#endif//MATF_RG_PROJECT_POINTSHADOWMAP_H
