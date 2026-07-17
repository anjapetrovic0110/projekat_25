#include "MainController.h"
#include "GUIController.h"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"

namespace app {

class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (!gui_controller->is_enabled()) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx * 0.6f, position.dy * 0.6f);
    }
}

void MainController::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    engine::graphics::OpenGL::enable_depth_testing();
    m_renderer.init(platform->window()->width(), platform->window()->height());

    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->Position = glm::vec3(0.0f, 0.7f, 5.0f);
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}

void MainController::draw() {
    m_renderer.render(m_scene, m_first_event_active, m_second_event_active);
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}

void MainController::update_camera() {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (gui_controller->is_enabled()) {
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
    float dt = platform->dt();
    if (platform->key(engine::platform::KeyId::KEY_E).state() == engine::platform::Key::State::JustPressed) {
        m_event_active = true;
        m_event_timer = 0.0f;

        m_first_event_active = false;
        m_second_event_active = false;
    }

    if (!m_event_active) {
        return;
    }

    m_event_timer += dt;

    if (m_event_timer >= 3.0f && m_event_timer < 6.0f && !m_first_event_active) {
        m_first_event_active = true;
    }

    if (m_event_timer >= 6.0f && m_event_timer < 10.0f && !m_second_event_active) {
        m_first_event_active = false;
        m_second_event_active = true;
    }

    if (m_event_timer >= 15.0f) {
        m_first_event_active = false;
        m_second_event_active = false;
        m_event_active = false;
    }
}

void MainController::update() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    m_renderer.resize(platform->window()->width(), platform->window()->height());
    update_camera();
    update_events();
}

void MainController::terminate() {
    m_renderer.terminate();
}

}// namespace app
