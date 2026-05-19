//
// Created by lenovo on 18. 5. 2026..
//

#include "../include/Renderer.h"

#include "GUIController.h"
#include "engine/core/Controller.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/resources/ResourcesController.hpp"

namespace app {
void Renderer::init(int w, int h) {
    width = w;
    height = h;

    msaa.init(w, h, 4);
    post.init(w, h);
}

void Renderer::render(Scene &scene, bool event_active, bool firstEvent_active, bool secondEvent_active) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Shader *shader = resources->shader("basic");
    engine::resources::Shader *shaderLight = resources->shader("flameShader");

    msaa.bind();

    shader->use();

    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);

    setup_lights(shader);
    update_lights(shader, event_active, firstEvent_active, secondEvent_active);
    scene.draw_objects(shader, firstEvent_active, secondEvent_active);

    shaderLight->use();
    shaderLight->set_mat4("projection", graphics->projection_matrix());
    shaderLight->set_mat4("view", graphics->camera()->view_matrix());

    glm::vec3 flameColor;
    if (firstEvent_active) {
        flameColor = glm::vec3(0.0f);
    } else if (secondEvent_active) {
        flameColor = glm::vec3(0.35f, 0.05f, 0.05f);
    } else {
        flameColor = glm::vec3(0.8f, 0.35f, 0.1f);
    }

    shaderLight->set_vec3("flameColor", flameColor);

    scene.draw_flames(shaderLight);

    msaa.unbind();
    msaa.resolve();
    post.render(msaa.getTexture());
}

void Renderer::setup_lights(engine::resources::Shader *shader) {
    shader->set_vec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    shader->set_vec3("dirLight.ambient", glm::vec3(0.1f));
    shader->set_vec3("dirLight.diffuse", glm::vec3(0.3f));
    shader->set_vec3("dirLight.specular", glm::vec3(0.4f));

    glm::vec3 positions[3] = {
            glm::vec3(1.0f, -0.7f, -6.0f) + glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec3(-1.0f, -0.7f, -6.0f) + glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, -0.7f, -7.0f) + glm::vec3(0.0f, 0.5f, 0.0f)};

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
void Renderer::update_lights(engine::resources::Shader *shader, bool event_active, bool firstEvent_active, bool secondEvent_active) {

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
            continue;
        }

        if (firstEvent_active) {
            shader->set_vec3(base + "ambient", glm::vec3(0.0f));
            shader->set_vec3(base + "diffuse", glm::vec3(0.0f));
            shader->set_vec3(base + "specular", glm::vec3(0.0f));
        } else if (secondEvent_active) {
            shader->set_vec3(base + "ambient", glm::vec3(0.08f, 0.02f, 0.02f));
            shader->set_vec3(base + "diffuse", glm::vec3(0.35f, 0.05f, 0.05f));
            shader->set_vec3(base + "specular", glm::vec3(0.2f));
        } else {
            shader->set_vec3(base + "ambient", glm::vec3(0.02f, 0.01f, 0.005f));
            shader->set_vec3(base + "diffuse", glm::vec3(0.8f, 0.35f, 0.1f));
            shader->set_vec3(base + "specular", glm::vec3(0.1f));
        }
    }
}

}// namespace app