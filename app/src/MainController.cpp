//
// Created by lenovo on 29. 3. 2026..
//

#include "../include/MainController.h"

#include "GUIController.h"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/Input.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"

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
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}

void MainController::setupLights(engine::resources::Shader *shader) {
    glm::vec3 positions[3] = {
            glm::vec3(1.0f, -0.7f, -6.0f),
            glm::vec3(-1.0f, -0.7f, -6.0f),
            glm::vec3(0.0f, -0.7f, -7.0f)};

    for (int i = 0; i < 3; i++) {
        std::string base = "pointLights[" + std::to_string(i) + "].";

        shader->set_vec3(base + "position", positions[i]);

        shader->set_vec3(base + "ambient", glm::vec3(0.02f, 0.01f, 0.005f));
        shader->set_vec3(base + "diffuse", glm::vec3(0.8f, 0.35f, 0.1f));
        shader->set_vec3(base + "specular", glm::vec3(0.1f));

        shader->set_float(base + "constant", 1.0f);
        shader->set_float(base + "linear", 0.09f);
        shader->set_float(base + "quadratic", 0.032f);
    }
}

void MainController::draw() {
    draw_statue();
    draw_hall();
    draw_torch(glm::vec3(1.0f, -0.7f, -6.0f));
    draw_torch(glm::vec3(-1.0f, -0.7f, -6.0f));
    draw_torch(glm::vec3(0.0f, -0.7f, -7.0f));
}

void MainController::draw_statue() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *statue = resources->model("statue");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);

    shader->set_vec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    shader->set_vec3("dirLight.ambient", glm::vec3(0.4f));
    shader->set_vec3("dirLight.diffuse", glm::vec3(0.3f));
    shader->set_vec3("dirLight.specular", glm::vec3(0.4f));
    update_lights(shader);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.65f, -6.0f));
    model = glm::scale(model, glm::vec3(0.001f));
    shader->set_mat4("model", model);

    statue->draw(shader);
}

void MainController::draw_hall() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *hall = resources->model("hall");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);
    update_lights(shader);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, -6.0f));
    model = glm::scale(model, glm::vec3(0.3f));
    shader->set_mat4("model", model);

    hall->draw(shader);
}

void MainController::draw_torch(glm::vec3 position) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *torch = resources->model("torch");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);
    setupLights(shader);
    update_lights(shader);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f));
    shader->set_mat4("model", model);

    torch->draw(shader);
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

void MainController::update_lights(engine::resources::Shader *shader) {
    auto gui_controller = engine::core::Controller::get<GUIController>();

    shader->set_vec3("dirLight.direction", glm::vec3(
                                                   gui_controller->dirDirection[0],
                                                   gui_controller->dirDirection[1],
                                                   gui_controller->dirDirection[2]));
    shader->set_vec3("dirLight.ambient", glm::vec3(
                                                 gui_controller->dirAmbient[0],
                                                 gui_controller->dirAmbient[1],
                                                 gui_controller->dirAmbient[2]));
    shader->set_vec3("dirLight.diffuse", glm::vec3(
                                                 gui_controller->dirDiffuse[0],
                                                 gui_controller->dirDiffuse[1],
                                                 gui_controller->dirDiffuse[2]));
    shader->set_vec3("dirLight.specular", glm::vec3(
                                                  gui_controller->dirSpecular[0],
                                                  gui_controller->dirSpecular[1],
                                                  gui_controller->dirSpecular[2]));

    for (int i = 0; i < 3; i++) {
        std::string base = "pointLights[" + std::to_string(i) + "].";
        if (!gui_controller->pointEnabled[i]) {
            shader->set_vec3(base + "ambient", glm::vec3(0.0f));
            shader->set_vec3(base + "diffuse", glm::vec3(0.0f));
            shader->set_vec3(base + "specular", glm::vec3(0.0f));
        } else {
            shader->set_vec3(base + "ambient", glm::vec3(
                                                       gui_controller->pointAmbient[0],
                                                       gui_controller->pointAmbient[1],
                                                       gui_controller->pointAmbient[2]));
            shader->set_vec3(base + "diffuse", glm::vec3(
                                                       gui_controller->pointDiffuse[0],
                                                       gui_controller->pointDiffuse[1],
                                                       gui_controller->pointDiffuse[2]));
            shader->set_vec3(base + "specular", glm::vec3(
                                                        gui_controller->pointSpecular[0],
                                                        gui_controller->pointSpecular[1],
                                                        gui_controller->pointSpecular[2]));
        }
    }
}

void MainController::update() {
    update_camera();
}

}// namespace app