//
// Created by lenovo on 2. 5. 2026..
//

#include "../include/PostProcessor.h"

#include "engine/resources/ResourcesController.hpp"

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

    GLuint VBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    shader = resources->shader("msaaShader");
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