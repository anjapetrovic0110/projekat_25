/**
 * @file Skybox.hpp
 * @brief Defines the Skybox class that serves as the interface for skyboxes rendering.
*/

#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <filesystem>
#include <utility>

namespace engine::resources {
/**
* @class Skybox
* @brief Represents a skyboxes object within the OpenGL context.
*/
class Skybox {
    friend class ResourcesController;

public:
    /**
    * @brief Returns the OpenGL ID of the skyboxes.
    * @returns The OpenGL ID of the skyboxes.
    */
    uint32_t vao() const {
        return m_vao;
    }

    /**
    * @brief Returns the OpenGL ID of the skyboxes texture.
    * @returns The OpenGL ID of the skyboxes texture.
    */
    uint32_t texture() const {
        return m_texture_id;
    }

    /**
    * @brief Destroys the skyboxes object in the OpenGL context.
    */
    void destroy();

private:
    Skybox() = default;

    uint32_t m_vao{0};
    uint32_t m_texture_id{0};
    std::filesystem::path m_path{};
    std::string m_name{};

    /**
    * @brief Constructs a Skybox object.
    * @param vao The OpenGL ID of the skyboxes.
    * @param texture_id The OpenGL ID of the skyboxes texture.
    * @param path The path to the skyboxes texture.
    * @param name The name of the skyboxes.
    */
    Skybox(uint32_t vao, uint32_t texture_id, std::filesystem::path path, std::string name)
        : m_vao(vao)
        , m_texture_id(texture_id)
        , m_path(std::move(path))
        , m_name(std::move(name)) {
    }
};

}// namespace engine::resources

#endif//SKYBOX_HPP
