
#ifndef MATF_RG_PROJECT_MAINCONTROLLER_H
#define MATF_RG_PROJECT_MAINCONTROLLER_H

#include "Renderer.h"
#include "engine/core/Controller.hpp"

namespace engine::resources {
class Shader;
}
namespace app {

class MainController : public engine::core::Controller {

    bool m_event_active = false;
    float m_event_timer = 0.0f;
    bool m_first_event_active = false;
    bool m_second_event_active = false;
    Renderer m_renderer;
    Scene m_scene;


    void initialize() override;
    bool loop() override;
    void draw() override;
    void begin_draw() override;
    void end_draw() override;
    void update() override;
    void terminate() override;
    void update_camera();
    void update_events();


public:
    std::string_view name() const override {
        return "app::MainController";
    }
};

}// namespace app

#endif//MATF_RG_PROJECT_MAINCONTROLLER_H
