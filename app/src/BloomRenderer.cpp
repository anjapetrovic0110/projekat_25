//
// Created by lenovo on 26. 4. 2026..
//

#include "BloomRenderer.h"

#include "../../engine/libs/glad/include/glad/glad.h"
#include "engine/resources/ResourcesController.hpp"

namespace app {

void BloomRenderer::initialize(int w, int h) {
    width = w;
    height = h;

    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

    glGenTextures(2, colorBuffers);
    for (int i = 0; i < 2; i++) {

        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
                     width, height, 0,
                     GL_RGBA, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D,
                               colorBuffers[i],
                               0);
    }

    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, rboDepth);

    unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);

    for (int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);

        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
                     width, height, 0,
                     GL_RGBA, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               pingpongColorbuffers[i],
                               0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BloomRenderer::begin_scene() {
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BloomRenderer::render() {
    bool horizontal = true;
    bool first = true;
    int amount = 5;

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

    engine::resources::Shader *blurShader = resources->shader("blurShader");
    engine::resources::Shader *finalShader = resources->shader("finalCombine");

    blurShader->use();
    blurShader->set_int("image", 0);

    for (int i = 0; i < amount; i++) {

        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
        glActiveTexture(GL_TEXTURE0);

        if (first) {
            glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
        } else {
            glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
        }

        blurShader->set_int("horizontal", horizontal);

        renderQuad();

        horizontal = !horizontal;

        if (first)
            first = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    finalShader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);

    finalShader->set_int("scene", 0);
    finalShader->set_int("bloomBlur", 1);
    finalShader->set_float("exposure", 1.0f);

    renderQuad();
}

void BloomRenderer::end_scene() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BloomRenderer::renderQuad() {

    if (quadVAO == 0) {
        float quadVertices[] = {
                -1.0f, 1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f,
                1.0f, -1.0f, 1.0f, 0.0f,

                -1.0f, 1.0f, 0.0f, 1.0f,
                1.0f, -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 1.0f, 1.0f
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);

        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices),
                     quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(float), (void *) 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(float), (void *) (2 * sizeof(float)));
    }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


}// namespace app
