#include "Renderer.h"
#include "GUIController.h"
#include "engine/core/Controller.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/resources/ResourcesController.hpp"

namespace app {
void Renderer::init(int w, int h) {
    m_width = w;
    m_height = h;

    m_msaa.init(w, h, 4);
    m_post.init(w, h);

    for (int i = 0; i < 3; i++) {
        m_shadow_maps[i].init();
    }

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto lighting_shader = resources->shader("basic");
    lighting_shader->use();
    lighting_shader->set_int("shadowMap0", 3);
    lighting_shader->set_int("shadowMap1", 4);
    lighting_shader->set_int("shadowMap2", 5);
    lighting_shader->set_float("far_plane", 50.0f);
}

void Renderer::resize(int width, int height) {
    if (width <= 0 || height <= 0 || (width == m_width && height == m_height)) {
        return;
    }
    m_width = width;
    m_height = height;
    m_msaa.resize(width, height);
    m_post.resize(width, height);
}

void Renderer::terminate() {
    m_msaa.terminate();
    m_post.terminate();
    for (auto &shadow_map: m_shadow_maps) {
        shadow_map.terminate();
    }
}

void Renderer::render_shadow_pass(Scene &scene, bool first_event_active, bool second_event_active) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto depth_shader = resources->shader("pointShadowDepthShader");

    float far_plane = 50.0f;
    glm::mat4 shadow_projection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, far_plane);

    depth_shader->use();
    depth_shader->set_float("far_plane", far_plane);

    for (int i = 0; i < 3; i++) {
        glm::vec3 light_position = m_light_positions[i];

        std::vector<glm::mat4> shadow_transforms = {
                shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),
                shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
                shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
                shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
                shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),
                shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)),
        };

        for (int j = 0; j < 6; j++) {
            depth_shader->set_mat4("shadowMatrices[" + std::to_string(j) + "]", shadow_transforms[j]);
        }

        depth_shader->set_vec3("lightPos", light_position);

        engine::graphics::OpenGL::set_viewport(m_shadow_maps[i].width(), m_shadow_maps[i].height());
        m_shadow_maps[i].bind();
        scene.draw_objects(depth_shader, first_event_active, second_event_active);
        m_shadow_maps[i].unbind();
    }

    engine::graphics::OpenGL::set_viewport(m_width, m_height);
}

void Renderer::render(Scene &scene, bool first_event_active, bool second_event_active) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto gui_controller = engine::core::Controller::get<GUIController>();

    render_shadow_pass(scene, first_event_active, second_event_active);
    if (gui_controller->msaa_enabled) {
        m_msaa.bind();
    } else {
        m_post.bind_plain();
    }
    engine::graphics::OpenGL::clear_color_and_depth_buffers();

    auto shader = resources->shader("basic");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);

    setup_lights(shader);
    update_lights(shader, first_event_active, second_event_active);

    engine::graphics::OpenGL::bind_cubemap_texture(m_shadow_maps[0].depth_cubemap(), 3);
    engine::graphics::OpenGL::bind_cubemap_texture(m_shadow_maps[1].depth_cubemap(), 4);
    engine::graphics::OpenGL::bind_cubemap_texture(m_shadow_maps[2].depth_cubemap(), 5);

    scene.draw_objects(shader, first_event_active, second_event_active);

    auto shader_light = resources->shader("flameShader");
    shader_light->use();
    shader_light->set_mat4("projection", graphics->projection_matrix());
    shader_light->set_mat4("view", graphics->camera()->view_matrix());

    glm::vec3 flame_color;
    if (first_event_active) {
        flame_color = glm::vec3(0.0f);
    } else if (second_event_active) {
        flame_color = glm::vec3(0.35f, 0.05f, 0.05f);
    } else {
        flame_color = glm::vec3(0.8f, 0.35f, 0.1f);
    }

    shader_light->set_vec3("flameColor", flame_color);
    scene.draw_flames(shader_light);

    draw_skybox();

    if (gui_controller->msaa_enabled) {
        m_msaa.unbind();
        m_msaa.resolve();
        m_post.render(m_msaa.get_texture());
    } else {
        m_post.unbind_plain();
        m_post.render(m_post.get_plain_texture());
    }
}

void Renderer::setup_lights(engine::resources::Shader *shader) {
    shader->set_vec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    shader->set_vec3("dirLight.ambient", glm::vec3(0.1f));
    shader->set_vec3("dirLight.diffuse", glm::vec3(0.3f));
    shader->set_vec3("dirLight.specular", glm::vec3(0.4f));

    for (int i = 0; i < 3; i++) {
        std::string base = "pointLights[" + std::to_string(i) + "].";

        shader->set_vec3(base + "position", m_light_positions[i]);

        shader->set_vec3(base + "ambient", glm::vec3(0.02f, 0.01f, 0.005f));
        shader->set_vec3(base + "diffuse", glm::vec3(0.8f, 0.35f, 0.1f));
        shader->set_vec3(base + "specular", glm::vec3(0.1f));

        shader->set_float(base + "constant", 1.0f);
        shader->set_float(base + "linear", 0.09f);
        shader->set_float(base + "quadratic", 0.032f);
    }
}

void Renderer::update_lights(engine::resources::Shader *shader, bool first_event_active, bool second_event_active) {

    auto gui_controller = engine::core::Controller::get<GUIController>();

    shader->set_vec3("dirLight.direction", glm::vec3(
                                                   gui_controller->dir_direction[0],
                                                   gui_controller->dir_direction[1],
                                                   gui_controller->dir_direction[2]));

    shader->set_vec3("dirLight.ambient", glm::vec3(
                                                 gui_controller->dir_ambient[0],
                                                 gui_controller->dir_ambient[1],
                                                 gui_controller->dir_ambient[2]));

    shader->set_vec3("dirLight.diffuse", glm::vec3(
                                                 gui_controller->dir_diffuse[0],
                                                 gui_controller->dir_diffuse[1],
                                                 gui_controller->dir_diffuse[2]));

    shader->set_vec3("dirLight.specular", glm::vec3(
                                                  gui_controller->dir_specular[0],
                                                  gui_controller->dir_specular[1],
                                                  gui_controller->dir_specular[2]));


    for (int i = 0; i < 3; i++) {
        std::string base = "pointLights[" + std::to_string(i) + "].";

        if (!gui_controller->point_enabled[i]) {
            shader->set_vec3(base + "ambient", glm::vec3(0.0f));
            shader->set_vec3(base + "diffuse", glm::vec3(0.0f));
            shader->set_vec3(base + "specular", glm::vec3(0.0f));
            continue;
        }

        if (first_event_active) {
            shader->set_vec3(base + "ambient", glm::vec3(0.0f));
            shader->set_vec3(base + "diffuse", glm::vec3(0.0f));
            shader->set_vec3(base + "specular", glm::vec3(0.0f));
        } else if (second_event_active) {
            shader->set_vec3(base + "ambient", glm::vec3(0.08f, 0.02f, 0.02f));
            shader->set_vec3(base + "diffuse", glm::vec3(0.35f, 0.05f, 0.05f));
            shader->set_vec3(base + "specular", glm::vec3(0.2f));
        } else {
            shader->set_vec3(base + "ambient", glm::vec3(gui_controller->point_ambient[0],
                                                         gui_controller->point_ambient[1],
                                                         gui_controller->point_ambient[2]));
            shader->set_vec3(base + "diffuse", glm::vec3(gui_controller->point_diffuse[0],
                                                         gui_controller->point_diffuse[1],
                                                         gui_controller->point_diffuse[2]));
            shader->set_vec3(base + "specular", glm::vec3(gui_controller->point_specular[0],
                                                          gui_controller->point_specular[1],
                                                          gui_controller->point_specular[2]));
        }
    }
}

void Renderer::draw_skybox() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    auto skybox = resources->skybox("space_skybox");
    auto shader = resources->shader("skyboxShader");

    graphics->draw_skybox(shader, skybox);
}

}// namespace app
