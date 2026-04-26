//
// Created by lenovo on 26. 4. 2026..
//

#ifndef MATF_RG_PROJECT_BLOOMRENDERER_H
#define MATF_RG_PROJECT_BLOOMRENDERER_H

namespace app {

class BloomRenderer {
public:
    void initialize(int width, int height);
    void begin_scene();
    void end_scene();
    void render();

private:
    unsigned int hdrFBO;
    unsigned int colorBuffers[2];

    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];

    unsigned int quadVAO = 0, quadVBO;

    int width, height;

    void renderQuad();
};

}// namespace app
#endif//MATF_RG_PROJECT_BLOOMRENDERER_H
