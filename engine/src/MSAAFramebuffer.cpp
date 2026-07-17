#include <engine/graphics/MSAAFramebuffer.hpp>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace engine::graphics {

void MSAAFramebuffer::init(int width, int height, int samples) {
    m_width = width;
    m_height = height;
    m_samples = samples;

    glGenFramebuffers(1, &m_msaa_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_msaa_fbo);
    glGenTextures(1, &m_color_buffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_color_buffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGBA16F, m_width, m_height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_color_buffer, 0);

    glGenRenderbuffers(1, &m_depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH24_STENCIL8, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_buffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("MSAA framebuffer incomplete!");
    }

    glGenFramebuffers(1, &m_resolve_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_resolve_fbo);
    glGenTextures(1, &m_resolved_texture);
    glBindTexture(GL_TEXTURE_2D, m_resolved_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_resolved_texture, 0);
    GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, draw_buffers);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MSAAFramebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_msaa_fbo);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MSAAFramebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MSAAFramebuffer::resolve() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msaa_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolve_fbo);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MSAAFramebuffer::resize(int width, int height) {
    if (width == m_width && height == m_height) {
        return;
    }
    int samples = m_samples;
    terminate();
    init(width, height, samples);
}

void MSAAFramebuffer::terminate() {
    glDeleteFramebuffers(1, &m_msaa_fbo);
    glDeleteFramebuffers(1, &m_resolve_fbo);
    glDeleteTextures(1, &m_color_buffer);
    glDeleteTextures(1, &m_resolved_texture);
    glDeleteRenderbuffers(1, &m_depth_buffer);
    m_msaa_fbo = 0;
    m_resolve_fbo = 0;
    m_color_buffer = 0;
    m_resolved_texture = 0;
    m_depth_buffer = 0;
}

}// namespace engine::graphics
