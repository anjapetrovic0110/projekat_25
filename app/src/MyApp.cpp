//
// Created by lenovo on 29. 3. 2026..
//

#include "../include/MyApp.h"
#include "../include/MainController.h"

namespace app {
void MyApp::app_setup() {
    auto main_controller = register_controller<app::MainController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
}
}// namespace app
