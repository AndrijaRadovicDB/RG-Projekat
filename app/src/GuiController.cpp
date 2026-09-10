//
// Created by andrija on 9. 9. 2026..
//

#include "../include/GuiController.hpp"

#include <imgui.h>
#include <engine/platform/PlatformController.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <../include/MainController.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace app {
    void GUIController::initialize() {
        set_enable(false);
    }

    void GUIController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_F2).state() == engine::platform::Key::State::JustPressed) {
            set_enable(!is_enabled());
        }
    }

    void GUIController::draw() {
        auto graphics        = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto main_controller = engine::core::Controller::get<MainController>();
        graphics->begin_gui();

        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
        ImGui::Begin("Lighting");

        ImGui::Text("Directional Light");
        ImGui::ColorEdit3("Directional Light Color", &main_controller->dirLight_color[0]);
        ImGui::SliderFloat3("Directional Light Position", &main_controller->dirLight_direction[0], -1.0f, 1.0f);

        ImGui::Separator();

        ImGui::Text("Point Light");
        ImGui::ColorEdit3("Point Light Color", &main_controller->pointLight_color[0]);
        ImGui::SliderFloat3("Point Light Direction", &main_controller->pointLight_position[0], -5.0f, 5.0f);

        ImGui::End();

        graphics->end_gui();
    }
} // app
