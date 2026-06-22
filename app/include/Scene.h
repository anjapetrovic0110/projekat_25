
#ifndef MATF_RG_PROJECT_SCENE_H
#define MATF_RG_PROJECT_SCENE_H
#include "engine/resources/Shader.hpp"

namespace app {

class Scene {
public:
    void draw_objects(engine::resources::Shader *shader, bool firstEvent_active, bool secondEvent_active);
    void draw_flames(engine::resources::Shader *shaderLight);

private:
    float statue_angle = 0.0f;

    void draw_statue(engine::resources::Shader *shader, bool firstEvent_active, bool secondEvent_active);
    void draw_hall(engine::resources::Shader *shader);
    void draw_torch(engine::resources::Shader *shader, glm::vec3 position);
    void draw_flame(engine::resources::Shader *shader, glm::vec3 position);
};

}// namespace app

#endif//MATF_RG_PROJECT_SCENE_H
