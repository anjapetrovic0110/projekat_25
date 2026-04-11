//
// Created by lenovo on 29. 3. 2026..
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_H
#define MATF_RG_PROJECT_MAINCONTROLLER_H
#include "engine/core/Controller.hpp"
#include "glm/vec3.hpp"

namespace app {

class MainController : public engine::core::Controller {
    void initialize() override;
    bool loop() override;
    void draw() override;
    void begin_draw() override;
    void end_draw() override;
    void update() override;
    void update_camera();
    void draw_statue();
    void draw_hall();
    void draw_torch(glm::vec3 position);


public:
    std::string_view name() const override {
        return "app::MainController";
    }
};

}// namespace app

#endif//MATF_RG_PROJECT_MAINCONTROLLER_H
