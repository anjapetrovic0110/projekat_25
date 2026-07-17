#ifndef ENGINE_GRAPHICS_POSTPROCESSOR_HPP
#define ENGINE_GRAPHICS_POSTPROCESSOR_HPP

#include <cstdint>
#include <engine/resources/Shader.hpp>

namespace engine::graphics {

class PostProcessor {
public:
    void init(int width, int height);
    void render(uint32_t hdr_texture);
    void bind_plain();
    void unbind_plain();
    void resize(int width, int height);
    void terminate();
    uint32_t get_plain_texture() const { return m_plain_texture; }

private:
    uint32_t m_quad_vao = 0;
    uint32_t m_quad_vbo = 0;
    engine::resources::Shader *m_shader = nullptr;
    int m_screen_width = 0;
    int m_screen_height = 0;
    uint32_t m_plain_depth = 0;
    uint32_t m_plain_fbo = 0;
    uint32_t m_plain_texture = 0;

    void create_plain_fbo();
};

}// namespace engine::graphics

#endif//ENGINE_GRAPHICS_POSTPROCESSOR_HPP
