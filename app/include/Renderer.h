//
// Created by lenovo on 18. 5. 2026..
//

#ifndef MATF_RG_PROJECT_RENDERER_H
#define MATF_RG_PROJECT_RENDERER_H
#include "PointShadowMap.h"
#include "PostProcessor.h"
#include "RenderTarget.h"
#include "Scene.h"

namespace app {

class Renderer {
public:
    void init(int w, int h);
    void render(Scene &scene, bool event_active, bool firstEvent_active, bool secondEvent_active);

private:
    MSAAFramebuffer msaa;
    PostProcessor post;
    PointShadowMap shadowMaps[3];
    int width;
    int height;

    glm::vec3 lightPositions[3] = {
            glm::vec3(1.0f, -0.2f, -6.0f),
            glm::vec3(-1.0f, -0.2f, -6.0f),
            glm::vec3(0.0f, -0.2f, -7.0f)};

    void render_shadow_pass(Scene &scene, bool firstEvent_active, bool secondEvent_active);
    void setup_lights(engine::resources::Shader *shader);
    void update_lights(engine::resources::Shader *shader, bool event_active, bool firstEvent_active, bool secondEvent_active);
};

}// namespace app

#endif//MATF_RG_PROJECT_RENDERER_H
