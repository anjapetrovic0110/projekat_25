#ifndef ENGINE_GRAPHICS_POINTSHADOWMAP_HPP
#define ENGINE_GRAPHICS_POINTSHADOWMAP_HPP

#include <cstdint>

namespace engine::graphics {

class PointShadowMap {
public:
    void init();
    void bind();
    void unbind();
    void terminate();
    uint32_t depth_cubemap() const { return m_depth_cubemap; }
    unsigned int width() const { return m_shadow_width; }
    unsigned int height() const { return m_shadow_height; }

private:
    uint32_t m_depth_cubemap = 0;
    uint32_t m_depth_map_fbo = 0;
    const unsigned int m_shadow_width = 1024;
    const unsigned int m_shadow_height = 1024;
};

}// namespace engine::graphics

#endif//ENGINE_GRAPHICS_POINTSHADOWMAP_HPP
