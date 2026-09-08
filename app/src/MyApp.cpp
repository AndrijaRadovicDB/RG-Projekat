//
// Created by andrija on 8. 9. 2026..
//

#include "../include/MyApp.hpp"

#include <MainController.hpp>
#include <spdlog/spdlog.h>

namespace app {
    void MyApp::app_setup() {
        auto main_controller = register_controller<MainController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());

        spdlog::info("App setup completed");
    }
} // app
