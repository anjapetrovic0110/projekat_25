//
// Created by lenovo on 21. 5. 2026..
//

#ifndef MATF_RG_PROJECT_POINTSHADOWMAP_H
#define MATF_RG_PROJECT_POINTSHADOWMAP_H


#include "../../engine/libs/glad/include/glad/glad.h"
namespace app {

class PointShadowMap {

public:
    GLuint depthCubemap;
    GLuint depthMapFBO;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    void init();
    void bind();
    void unbind();
};

}// namespace app

#endif//MATF_RG_PROJECT_POINTSHADOWMAP_H
