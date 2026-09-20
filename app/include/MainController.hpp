//
// Created by andrija on 8. 9. 2026..
//

#ifndef MATF_RG_PROJECT_MYCONTROLLER_HPP
#define MATF_RG_PROJECT_MYCONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <functional>
#include <glm/vec3.hpp>
#include <queue>
#include <engine/resources/PostProcessing.hpp>
#include <engine/resources/ResourcesController.hpp>

namespace app {
    class MainController : public engine::core::Controller {
        void initialize() override;

        bool loop() override;

        void update_camera();

        void update_events();

        void update() override;

        void resize(int width, int height);

        void draw_tree();

        void draw_tree2();

        void draw_cauldron();

        void draw_terrain();

        void draw_wizard();

        void draw_skybox();

        void begin_draw() override;

        void end_draw() override;

        void draw() override;

        struct Event {
            float delay;
            std::function<void()> action;
        };

        std::unique_ptr<engine::resources::PostProcessing> m_post_processing;

    public:
        std::string_view name() const override {
            return "app::MainController";
        }

        glm::vec3 dir_light_direction = glm::vec3(-0.2f, -1.0f, 0.01f);
        glm::vec3 dir_light_color     = glm::vec3(0.3f, 0.3f, 0.3f);

        glm::vec3 point_light_position = glm::vec3(1.5f, 1.0f, 1.0f);
        glm::vec3 point_light_color    = glm::vec3(0.3f, 0.3f, 0.3f);

        std::queue<Event> events;

        bool event_chain_active = false;
        float event_timer       = 0.0f;

        bool wizard_visible  = true;
        bool scene_red       = false;
        bool show_event_text = false;
        bool event_state     = false;
    };
} // namespace app

#endif//MATF_RG_PROJECT_MYCONTROLLER_HPP
