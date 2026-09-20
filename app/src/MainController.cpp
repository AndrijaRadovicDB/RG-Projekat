//
// Created by andrija on 8. 9. 2026..
//

#include "MainController.hpp"

#include <GuiController.hpp>
#include <engine/core/Controller.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {
class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (!gui_controller->show_lighting()) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx, position.dy);
    }
}

void MainController::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    engine::graphics::OpenGL::enable_depth_testing();

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    m_post_processing = std::make_unique<engine::resources::PostProcessing>(1200, 800);
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}

void MainController::update_camera() {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (gui_controller->show_lighting()) {
        return;
    }

    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    float dt = platform->dt();

    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
    }
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
    }
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
    }
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
    }
}

void MainController::update_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    if (!event_chain_active && platform->key(engine::platform::KeyId::KEY_SPACE).state() ==
                                       engine::platform::Key::State::JustPressed) {
        event_chain_active = true;
        event_timer = 0.0f;

        wizard_visible = true;
        scene_red = false;
        show_event_text = false;

        events.push({1.0f, [this]() {
                         wizard_visible = false;
                         show_event_text = true;
                     }});

        events.push({2.0f, [this]() {
                         scene_red = true;
                     }});
    }

    if (!event_chain_active) {
        return;
    }

    if (events.empty()) {
        event_chain_active = false;
        return;
    }

    event_timer += platform->dt();
    Event &current_event = events.front();

    if (event_timer >= current_event.delay) {
        current_event.action();
        events.pop();
        event_timer = 0.0f;
    }
}

void MainController::update() {
    update_camera();
    update_events();
}
    
void MainController::resize(int width, int height) {
    m_post_processing->resize(width, height);
}

void MainController::draw_tree() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *tree = resources->model("tree");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);
    if (scene_red) {
        shader->set_vec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    } else {
        shader->set_vec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    }

    shader->set_vec3("dirLight_direction", dir_light_color);
    shader->set_vec3("dirLight_color", dir_light_color);

    shader->set_vec3("pointLight_position", point_light_color);
    shader->set_vec3("pointLight_color", point_light_color);
    shader->set_float("pointLight_const", 1.0f);
    shader->set_float("pointLight_linear", 0.09f);
    shader->set_float("pointLight_quadratic", 0.032f);

    std::vector<glm::vec3> treePositions;
    float radius = 2.0f;
    int num_of_trees = 6;

    for (int i = 0; i < num_of_trees; i++) {
        float angle = 2.0f * glm::pi<float>() * i / num_of_trees;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        treePositions.push_back(glm::vec3(x, -1.1f, z));
    }

    for (const auto &position: treePositions) {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(0.25f));

        shader->set_mat4("model", model);

        tree->draw(shader);
    }
}

void MainController::draw_tree2() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *tree2 = resources->model("tree2");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);
    if (scene_red) {
        shader->set_vec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    } else {
        shader->set_vec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    }

    shader->set_vec3("dirLight_direction", dir_light_color);
    shader->set_vec3("dirLight_color", dir_light_color);

    shader->set_vec3("pointLight_position", point_light_color);
    shader->set_vec3("pointLight_color", point_light_color);
    shader->set_float("pointLight_const", 1.0f);
    shader->set_float("pointLight_linear", 0.09f);
    shader->set_float("pointLight_quadratic", 0.032f);

    std::vector<glm::vec3> treePositions;
    float radius = 4.0f;
    int num_of_trees = 6;

    for (int i = 0; i < num_of_trees; i++) {
        float angle = 2.0f * glm::pi<float>() * i / num_of_trees + glm::radians(30.0f);
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        treePositions.push_back(glm::vec3(x, -1.0f, z));
    }

    for (const auto &position: treePositions) {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(0.0025f));

        shader->set_mat4("model", model);

        tree2->draw(shader);
    }
}

void MainController::draw_wizard() {
    if (!wizard_visible) {
        return;
    }

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *wizard = resources->model("wizard");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();

    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);
    if (scene_red) {
        shader->set_vec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    } else {
        shader->set_vec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    }

    shader->set_vec3("dirLight_direction", dir_light_color);
    shader->set_vec3("dirLight_color", dir_light_color);

    shader->set_vec3("pointLight_position", point_light_color);
    shader->set_vec3("pointLight_color", point_light_color);
    shader->set_float("pointLight_const", 1.0f);
    shader->set_float("pointLight_linear", 0.09f);
    shader->set_float("pointLight_quadratic", 0.032f);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.25f, -0.45f, -0.55f));
    model = glm::scale(model, glm::vec3(0.0075));

    shader->set_mat4("model", model);

    wizard->draw(shader);
}

void MainController::draw_cauldron() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *cauldron = resources->model("cauldron");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();

    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);
    if (scene_red) {
        shader->set_vec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    } else {
        shader->set_vec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    }

    shader->set_vec3("dirLight_direction", dir_light_color);
    shader->set_vec3("dirLight_color", dir_light_color);

    shader->set_vec3("pointLight_position", point_light_color);
    shader->set_vec3("pointLight_color", point_light_color);
    shader->set_float("pointLight_const", 1.0f);
    shader->set_float("pointLight_linear", 0.09f);
    shader->set_float("pointLight_quadratic", 0.032f);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3));

    shader->set_mat4("model", model);

    cauldron->draw(shader);
}

void MainController::draw_terrain() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *terrain = resources->model("terrain");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();

    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);
    if (scene_red) {
        shader->set_vec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    } else {
        shader->set_vec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    }

    shader->set_vec3("dirLight_direction", dir_light_color);
    shader->set_vec3("dirLight_color", dir_light_color);

    shader->set_vec3("pointLight_position", point_light_color);
    shader->set_vec3("pointLight_color", point_light_color);
    shader->set_float("pointLight_const", 1.0f);
    shader->set_float("pointLight_linear", 0.09f);
    shader->set_float("pointLight_quadratic", 0.032f);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, -2.8f, 0.0f));
    model = glm::scale(model, glm::vec3(0.25));

    shader->set_mat4("model", model);

    terrain->draw(shader);
}

void MainController::draw_skybox() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox = resources->skybox("forest");
    auto shader = resources->shader("skybox");
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->draw_skybox(shader, skybox);
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}

void MainController::draw() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

    m_post_processing->begin();

    draw_tree();
    draw_tree2();
    draw_cauldron();
    draw_wizard();
    draw_terrain();
    draw_skybox();

    m_post_processing->end(resources->shader("red_tint"));
}
}// namespace app
