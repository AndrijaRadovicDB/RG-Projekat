//
// Created by andrija on 8. 9. 2026..
//

#ifndef MATF_RG_PROJECT_MYCONTROLLER_HPP
#define MATF_RG_PROJECT_MYCONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <glm/vec3.hpp>

namespace app {
    class MainController : public engine::core::Controller {
        void initialize() override;

        bool loop() override;

        void update_camera();

        void update() override;

        void draw_tree();

        void draw_skybox();

        void begin_draw() override;

        void end_draw() override;

        void draw() override;

    public:
        std::string_view name() const override {
            return "app::MainController";
        }

        glm::vec3 dirLight_direction = glm::vec3(-0.2f, -1.0f, 0.01f);
        glm::vec3 dirLight_color     = glm::vec3(0.3f, 0.3f, 0.3f);

        glm::vec3 pointLight_position = glm::vec3(1.5f, 1.0f, 1.0f);
        glm::vec3 pointLight_color    = glm::vec3(0.3f, 0.3f, 0.3f);
    };
} // app

#endif //MATF_RG_PROJECT_MYCONTROLLER_HPP
