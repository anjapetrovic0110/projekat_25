
#ifndef MATF_RG_PROJECT_GUICONTROLLER_H
#define MATF_RG_PROJECT_GUICONTROLLER_H

#include "engine/core/Controller.hpp"

namespace app {

class GUIController : public engine::core::Controller {
public:
    float dir_direction[3] = {-0.2f, -1.0f, -0.3f};
    float dir_ambient[3] = {0.4f, 0.4f, 0.4f};
    float dir_diffuse[3] = {0.3f, 0.3f, 0.3f};
    float dir_specular[3] = {0.4f, 0.4f, 0.4f};
    float point_ambient[3] = {0.02f, 0.01f, 0.005f};
    float point_diffuse[3] = {0.8f, 0.35f, 0.1f};
    float point_specular[3] = {0.1f, 0.1f, 0.1f};
    bool point_enabled[3] = {true, true, true};
    bool msaa_enabled = true;

    std::string_view name() const override {
        return "app::GUIController";
    }

private:
    void initialize() override;
    void draw() override;
    void poll_events() override;
};

}// namespace app
#endif//MATF_RG_PROJECT_GUICONTROLLER_H
