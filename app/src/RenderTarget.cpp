
#include "../include/RenderTarget.h"
#include <iostream>

namespace app {

void MSAAFramebuffer::init(int w, int h, int s) {
    width = w;
    height = h;
    samples = s;

    glGenFramebuffers(1, &msaaFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO);

    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorBuffer);

    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                            samples,
                            GL_RGBA16F,
                            width,
                            height,
                            GL_TRUE);

    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE,
                           colorBuffer,
                           0);


    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);

    glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                                     samples,
                                     GL_DEPTH24_STENCIL8,
                                     width,
                                     height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER,
                              depthBuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "MSAA framebuffer incomplete!" << std::endl;
    }

    glGenFramebuffers(1, &resolveFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, resolveFBO);

    glGenTextures(1, &resolvedTexture);
    glBindTexture(GL_TEXTURE_2D, resolvedTexture);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA16F,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_FLOAT,
                 nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           resolvedTexture,
                           0);

    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MSAAFramebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MSAAFramebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MSAAFramebuffer::resolve() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resolveFBO);

    glBlitFramebuffer(0, 0, width, height,
                      0, 0, width, height,
                      GL_COLOR_BUFFER_BIT,
                      GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}// namespace app