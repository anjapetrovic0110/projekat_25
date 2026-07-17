#include <engine/graphics/PointShadowMap.hpp>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace engine::graphics {

void PointShadowMap::init() {
    glGenFramebuffers(1, &m_depth_map_fbo);
    glGenTextures(1, &m_depth_cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_depth_cubemap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     m_shadow_width, m_shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, m_depth_map_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth_cubemap, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Point shadow framebuffer incomplete!");
    }
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointShadowMap::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_depth_map_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void PointShadowMap::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointShadowMap::terminate() {
    glDeleteFramebuffers(1, &m_depth_map_fbo);
    glDeleteTextures(1, &m_depth_cubemap);
    m_depth_map_fbo = 0;
    m_depth_cubemap = 0;
}

}// namespace engine::graphics
