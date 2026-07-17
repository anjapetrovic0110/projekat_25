#include <engine/core/Controller.hpp>
#include <engine/graphics/PostProcessor.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace engine::graphics {

void PostProcessor::init(int width, int height) {
    m_screen_height = height;
    m_screen_width = width;
    float quad_vertices[] = {
            -1, 1, 0, 1,
            -1, -1, 0, 0,
            1, -1, 1, 0,
            -1, 1, 0, 1,
            1, -1, 1, 0,
            1, 1, 1, 1};
    glGenVertexArrays(1, &m_quad_vao);
    glGenBuffers(1, &m_quad_vbo);
    glBindVertexArray(m_quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    m_shader = resources->shader("msaaShader");
    create_plain_fbo();
}

void PostProcessor::create_plain_fbo() {
    glGenFramebuffers(1, &m_plain_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_plain_fbo);
    glGenTextures(1, &m_plain_texture);
    glBindTexture(GL_TEXTURE_2D, m_plain_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_screen_width, m_screen_height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_plain_texture, 0);
    glGenRenderbuffers(1, &m_plain_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, m_plain_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_screen_width, m_screen_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_plain_depth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Plain framebuffer incomplete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::bind_plain() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_plain_fbo);
    glViewport(0, 0, m_screen_width, m_screen_height);
}

void PostProcessor::unbind_plain() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::render(uint32_t hdr_texture) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, m_screen_width, m_screen_height);
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader->use();
    m_shader->set_int("screenTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdr_texture);
    glBindVertexArray(m_quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

void PostProcessor::resize(int width, int height) {
    if (width == m_screen_width && height == m_screen_height) {
        return;
    }
    glDeleteFramebuffers(1, &m_plain_fbo);
    glDeleteTextures(1, &m_plain_texture);
    glDeleteRenderbuffers(1, &m_plain_depth);
    m_plain_fbo = 0;
    m_plain_texture = 0;
    m_plain_depth = 0;
    m_screen_width = width;
    m_screen_height = height;
    create_plain_fbo();
}

void PostProcessor::terminate() {
    glDeleteFramebuffers(1, &m_plain_fbo);
    glDeleteTextures(1, &m_plain_texture);
    glDeleteRenderbuffers(1, &m_plain_depth);
    glDeleteVertexArrays(1, &m_quad_vao);
    glDeleteBuffers(1, &m_quad_vbo);
    m_plain_fbo = 0;
    m_plain_texture = 0;
    m_plain_depth = 0;
    m_quad_vao = 0;
    m_quad_vbo = 0;
    m_shader = nullptr;
}

}// namespace engine::graphics
