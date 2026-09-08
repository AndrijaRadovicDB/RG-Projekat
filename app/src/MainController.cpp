//
// Created by andrija on 8. 9. 2026..
//

#include "../include/MainController.hpp"

#include <engine/core/Controller.hpp>
#include <engine/platform/PlatformController.hpp>

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
