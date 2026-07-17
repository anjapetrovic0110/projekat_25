#ifndef ENGINE_GRAPHICS_MSAAFRAMEBUFFER_HPP
#define ENGINE_GRAPHICS_MSAAFRAMEBUFFER_HPP

#include <cstdint>

namespace engine::graphics {

class MSAAFramebuffer {
public:
    void init(int width, int height, int samples);
    void bind();
    void unbind();
    void resolve();
    void resize(int width, int height);
    void terminate();
    uint32_t get_texture() const { return m_resolved_texture; }

private:
    int m_width = 0;
    int m_height = 0;
    int m_samples = 0;
    uint32_t m_msaa_fbo = 0;
    uint32_t m_color_buffer = 0;
    uint32_t m_depth_buffer = 0;
    uint32_t m_resolve_fbo = 0;
    uint32_t m_resolved_texture = 0;
};

}// namespace engine::graphics

#endif//ENGINE_GRAPHICS_MSAAFRAMEBUFFER_HPP
