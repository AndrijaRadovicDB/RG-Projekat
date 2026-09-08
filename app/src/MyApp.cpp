//
// Created by andrija on 8. 9. 2026..
//

#include "../include/MyApp.hpp"

#include <MainController.hpp>
#include <spdlog/spdlog.h>
#include <GuiController.hpp>

namespace app {
    void MyApp::app_setup() {
        auto main_controller = register_controller<MainController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        auto gui_controller = register_controller<GUIController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        main_controller->before(gui_controller);

        spdlog::info("App setup completed");
    }
} // app
