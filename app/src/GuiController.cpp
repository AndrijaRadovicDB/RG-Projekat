//
// Created by andrija on 9. 9. 2026..
//

#include "GuiController.hpp"

#include <MainController.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace app {
void GUIController::initialize() {
    set_enable(true);
}

void GUIController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_F2).state() == engine::platform::Key::State::JustPressed) {
        m_show_lighting = !m_show_lighting;
    }
}

void GUIController::draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto main_controller = engine::core::Controller::get<MainController>();
    graphics->begin_gui();

    if (m_show_lighting) {
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
        ImGui::Begin("Lighting");

        ImGui::Text("Directional Light");
        ImGui::ColorEdit3("Directional Light Color", &main_controller->dir_light_color[0]);
        ImGui::SliderFloat3("Directional Light Position", &main_controller->dir_light_direction[0], -1.0f, 1.0f);

        ImGui::Separator();

        ImGui::Text("Point Light");
        ImGui::ColorEdit3("Point Light Color", &main_controller->point_light_color[0]);
        ImGui::SliderFloat3("Point Light Direction", &main_controller->point_light_position[0], -5.0f, 5.0f);

        ImGui::End();
    }

    if (main_controller->show_event_text) {
        ImGui::SetNextWindowPos(ImVec2(600, 200), ImGuiCond_Always);

        ImGui::Begin("Ritual", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("THE BLOOD MOON HAS BEGUN!");
        ImGui::Text("The wizard has dissapeared...");

        ImGui::End();
    }

    graphics->end_gui();
}
}// namespace app
