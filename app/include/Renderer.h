//
// Created by lenovo on 18. 5. 2026..
//

#ifndef MATF_RG_PROJECT_RENDERER_H
#define MATF_RG_PROJECT_RENDERER_H
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

    int width;
    int height;
    void setup_lights(engine::resources::Shader *shader);
    void update_lights(engine::resources::Shader *shader, bool event_active, bool firstEvent_active, bool secondEvent_active);
};

}// namespace app

#endif//MATF_RG_PROJECT_RENDERER_H
