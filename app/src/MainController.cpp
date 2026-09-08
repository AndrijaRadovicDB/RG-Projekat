//
// Created by andrija on 8. 9. 2026..
//

#include "../include/MainController.hpp"

#include <engine/core/Controller.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {
    void MainController::initialize() {
        spdlog::info("MainController initialized");
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }

    void MainController::draw_fish() {
        auto resources                  = engine::core::Controller::get<engine::resources::ResourcesController>();
        engine::resources::Model *model = resources->model("fish");
    }

    void MainController::draw() {
        draw_fish();
    }
} // app
