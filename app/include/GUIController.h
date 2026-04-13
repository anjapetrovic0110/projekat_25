//
// Created by lenovo on 12. 4. 2026..
//

#ifndef MATF_RG_PROJECT_GUICONTROLLER_H
#define MATF_RG_PROJECT_GUICONTROLLER_H
#include "engine/core/Controller.hpp"

namespace app {

class GUIController : public engine::core::Controller {
public:
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
