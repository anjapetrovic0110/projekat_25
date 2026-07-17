#include "Scene.h"
#include "engine/resources/ResourcesController.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace app {
void Scene::draw_objects(engine::resources::Shader *shader, bool first_event_active, bool second_event_active) {

    draw_statue(shader, first_event_active, second_event_active);
    draw_hall(shader);
    draw_torch(shader, glm::vec3(1.0f, -0.7f, -6.0f));
    draw_torch(shader, glm::vec3(-1.0f, -0.7f, -6.0f));
    draw_torch(shader, glm::vec3(0.0f, -0.7f, -7.0f));
}

void Scene::draw_flames(engine::resources::Shader *shader_light) {

    draw_flame(shader_light, glm::vec3(1.0f, -0.7f, -6.0f));
    draw_flame(shader_light, glm::vec3(-1.0f, -0.7f, -6.0f));
    draw_flame(shader_light, glm::vec3(0.0f, -0.7f, -7.0f));
}

void Scene::draw_statue(engine::resources::Shader *shader, bool first_event_active, bool second_event_active) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto statue = resources->model("statue");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.65f, -6.0f));

    if (first_event_active || second_event_active) {
        m_statue_angle += 0.8f;
        if (m_statue_angle > 360.0f) {
            m_statue_angle -= 360.0f;
        }
    }

    model = glm::rotate(model, glm::radians(m_statue_angle),
                        glm::vec3(0.0f, 1.0f, 0.0f));

    model = glm::scale(model, glm::vec3(0.001f));

    shader->set_mat4("model", model);
    statue->draw(shader);
}

void Scene::draw_hall(engine::resources::Shader *shader) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto hall = resources->model("hall");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, -6.0f));
    model = glm::scale(model, glm::vec3(0.3f));

    shader->set_mat4("model", model);
    hall->draw(shader);
}

void Scene::draw_torch(engine::resources::Shader *shader, glm::vec3 position) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto torch = resources->model("torch");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(-90.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f));

    shader->set_mat4("model", model);
    torch->draw(shader);
}

void Scene::draw_flame(engine::resources::Shader *shader, glm::vec3 position) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto lightBall = resources->model("light ball");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0.0f, 0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(0.6f));

    shader->set_mat4("model", model);
    lightBall->draw(shader);
}
}// namespace app
