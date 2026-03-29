//
// Created by lenovo on 29. 3. 2026..
//

#include "../include/MainController.h"

#include "engine/platform/Input.hpp"
#include "engine/platform/PlatformController.hpp"

namespace app {

    void MainController::initialize() {

    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }
    } // app