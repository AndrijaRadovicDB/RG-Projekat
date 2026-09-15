//
// Created by andrija on 9. 9. 2026..
//

#ifndef MATF_RG_PROJECT_GUICONTROLLER_HPP
#define MATF_RG_PROJECT_GUICONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
class GUIController : public engine::core::Controller {
    void initialize() override;

    void draw() override;

    void poll_events() override;

    bool m_show_lighting = false;

public:
    std::string_view name() const override {
        return "app::GUIController";
    }

    bool show_lighting() const {
        return m_show_lighting;
    }
};
}// namespace app

#endif//MATF_RG_PROJECT_GUICONTROLLER_HPP
