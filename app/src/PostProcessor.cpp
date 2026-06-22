
#include "../include/PostProcessor.h"

#include "engine/resources/ResourcesController.hpp"

#include <iostream>

namespace app {

void PostProcessor::init(int w, int h) {
    screenHeight = h;
    screenWidth = w;
    float quadVertices[] = {
            -1, 1, 0, 1,
            -1, -1, 0, 0,
            1, -1, 1, 0,

            -1, 1, 0, 1,
            1, -1, 1, 0,
            1, 1, 1, 1};
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    shader = resources->shader("msaaShader");

    create_plainFBO();
}
void PostProcessor::create_plainFBO() {

    glGenFramebuffers(1, &plainFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, plainFBO);
    glGenTextures(1, &plainTexture);
    glBindTexture(GL_TEXTURE_2D, plainTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, plainTexture, 0);
    glGenRenderbuffers(1, &plainDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, plainDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, plainDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Plain framebuffer incomplete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::bind_plain() {
    glBindFramebuffer(GL_FRAMEBUFFER, plainFBO);
    glViewport(0, 0, screenWidth, screenHeight);
}

void PostProcessor::unbind_plain() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void PostProcessor::render(GLuint hdrTexture) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, screenWidth, screenHeight);

    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();
    shader->set_int("screenTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}
}// namespace app