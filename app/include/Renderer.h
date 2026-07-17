
#ifndef MATF_RG_PROJECT_RENDERER_H
#define MATF_RG_PROJECT_RENDERER_H

#include "Scene.h"
#include "engine/graphics/MSAAFramebuffer.hpp"
#include "engine/graphics/PointShadowMap.hpp"
#include "engine/graphics/PostProcessor.hpp"

namespace app {

class Renderer {
public:
    void init(int w, int h);
    void resize(int width, int height);
    void terminate();
    void render(Scene &scene, bool first_event_active, bool second_event_active);

private:
    engine::graphics::MSAAFramebuffer m_msaa;
    engine::graphics::PostProcessor m_post;
    engine::graphics::PointShadowMap m_shadow_maps[3];
    int m_width = 0;
    int m_height = 0;

    glm::vec3 m_light_positions[3] = {
            glm::vec3(1.0f, -0.2f, -6.0f),
            glm::vec3(-1.0f, -0.2f, -6.0f),
            glm::vec3(0.0f, -0.2f, -7.0f)};

    void render_shadow_pass(Scene &scene, bool first_event_active, bool second_event_active);
    void setup_lights(engine::resources::Shader *shader);
    void update_lights(engine::resources::Shader *shader, bool first_event_active, bool second_event_active);
    void draw_skybox();
};

}// namespace app

#endif//MATF_RG_PROJECT_RENDERER_H
