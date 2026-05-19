//
// Created by lenovo on 29. 3. 2026..
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_H
#define MATF_RG_PROJECT_MAINCONTROLLER_H

#include "PostProcessor.h"
#include "RenderTarget.h"
#include "Renderer.h"
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
    Renderer renderer;
    Scene scene;


    void initialize() override;
    bool loop() override;
    void draw() override;
    void begin_draw() override;
    void end_draw() override;
    void update() override;
    void update_camera();
    void update_events();


public:
    std::string_view name() const override {
        return "app::MainController";
    }
};

}// namespace app

#endif//MATF_RG_PROJECT_MAINCONTROLLER_H
