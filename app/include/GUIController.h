//
// Created by lenovo on 12. 4. 2026..
//

#ifndef MATF_RG_PROJECT_GUICONTROLLER_H
#define MATF_RG_PROJECT_GUICONTROLLER_H
#include "engine/core/Controller.hpp"

namespace app {

class GUIController : public engine::core::Controller {
public:
    float dirDirection[3] = {-0.2f, -1.0f, -0.3f};
    float dirAmbient[3] = {0.4f, 0.4f, 0.4f};
    float dirDiffuse[3] = {0.3f, 0.3f, 0.3f};
    float dirSpecular[3] = {0.4f, 0.4f, 0.4f};
    float pointAmbient[3] = {0.02f, 0.01f, 0.005f};
    float pointDiffuse[3] = {0.8f, 0.35f, 0.1f};
    float pointSpecular[3] = {0.1f, 0.1f, 0.1f};
    bool pointEnabled[3] = {true, true, true};
    bool msaaEnabled = true;

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
