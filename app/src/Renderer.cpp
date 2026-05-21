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

    for (int i = 0; i < 3; i++) {
        shadowMaps[i].init();
    }

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto lightingShader = resources->shader("basic");
    lightingShader->use();
    lightingShader->set_int("shadowMap0", 3);
    lightingShader->set_int("shadowMap1", 4);
    lightingShader->set_int("shadowMap2", 5);
    lightingShader->set_float("far_plane", 50.0f);
}

void Renderer::render_shadow_pass(Scene &scene, bool firstEvent_active, bool secondEvent_active) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto depthShader = resources->shader("pointShadowDepthShader");

    float farPlane = 50.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, farPlane);

    depthShader->use();
    depthShader->set_float("far_plane", farPlane);

    for (int i = 0; i < 3; i++) {
        glm::vec3 lp = lightPositions[i];

        std::vector<glm::mat4> shadowTransforms = {
                shadowProj * glm::lookAt(lp, lp + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),
                shadowProj * glm::lookAt(lp, lp + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
                shadowProj * glm::lookAt(lp, lp + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
                shadowProj * glm::lookAt(lp, lp + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
                shadowProj * glm::lookAt(lp, lp + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),
                shadowProj * glm::lookAt(lp, lp + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)),
        };

        for (int j = 0; j < 6; j++)
            depthShader->set_mat4("shadowMatrices[" + std::to_string(j) + "]", shadowTransforms[j]);

        depthShader->set_vec3("lightPos", lp);

        glViewport(0, 0, 1024, 1024);
        shadowMaps[i].bind();
        scene.draw_objects(depthShader, firstEvent_active, secondEvent_active);
        shadowMaps[i].unbind();
    }

    glViewport(0, 0, width, height);
}

void Renderer::render(Scene &scene, bool event_active, bool firstEvent_active, bool secondEvent_active) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto gui_controller = engine::core::Controller::get<GUIController>();

    render_shadow_pass(scene, firstEvent_active, secondEvent_active);
    if (gui_controller->msaaEnabled) {
        msaa.bind();
    } else {
        post.bind_plain();
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto shader = resources->shader("basic");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);

    setup_lights(shader);
    update_lights(shader, event_active, firstEvent_active, secondEvent_active);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMaps[0].depthCubemap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMaps[1].depthCubemap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMaps[2].depthCubemap);

    scene.draw_objects(shader, firstEvent_active, secondEvent_active);

    auto shaderLight = resources->shader("flameShader");
    shaderLight->use();
    shaderLight->set_mat4("projection", graphics->projection_matrix());
    shaderLight->set_mat4("view", graphics->camera()->view_matrix());

    glm::vec3 flameColor;
    if (firstEvent_active) flameColor = glm::vec3(0.0f);
    else if (secondEvent_active)
        flameColor = glm::vec3(0.35f, 0.05f, 0.05f);
    else
        flameColor = glm::vec3(0.8f, 0.35f, 0.1f);

    shaderLight->set_vec3("flameColor", flameColor);
    scene.draw_flames(shaderLight);

    if (gui_controller->msaaEnabled) {
        msaa.unbind();
        msaa.resolve();
        post.render(msaa.getTexture());
    } else {
        post.unbind_plain();
        post.render(post.getPlainTexture());
    }
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