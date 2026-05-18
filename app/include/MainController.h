//
// Created by lenovo on 29. 3. 2026..
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_H
#define MATF_RG_PROJECT_MAINCONTROLLER_H

#include "PostProcessor.h"
#include "RenderTarget.h"
#include "engine/core/Controller.hpp"
#include "glm/vec3.hpp"

namespace engine::resources {
class Shader;
}
namespace app {

class MainController : public engine::core::Controller {

    bool event_active = false;
    float event_timer = 0.0f;
    bool firstEvent_active = false;
    bool secondEvent_active = false;
    float statue_angle = 0.0f;
    MSAAFramebuffer msaa;
    PostProcessor post;

    void initialize() override;
    bool loop() override;
    void draw() override;
    void begin_draw() override;
    void end_draw() override;
    void update() override;
    void update_camera();
    void update_lights_gui(engine::resources::Shader *shader);
    void update_lights_event(engine::resources::Shader *shader);
    void update_events();
    void draw_statue(engine::resources::Shader *shader);
    void draw_hall(engine::resources::Shader *shader);
    void draw_torch(engine::resources::Shader *shader, glm::vec3 position);
    void draw_flame(glm::vec3 position);
    void setup_lights(engine::resources::Shader *shader);

public:
    std::string_view name() const override {
        return "app::MainController";
    }
};

}// namespace app

#endif//MATF_RG_PROJECT_MAINCONTROLLER_H
