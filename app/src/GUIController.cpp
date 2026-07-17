#include "GUIController.h"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/platform/PlatformController.hpp"
#include "imgui.h"

namespace app {

void GUIController::initialize() {
    set_enable(false);
}

void GUIController::draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();

    graphics->begin_gui();

    ImGui::Begin("Settings");
    ImGui::Text("Camera position: (%f, %f, %f)", camera->Position.x, camera->Position.y, camera->Position.z);

    ImGui::Spacing();
    ImGui::Text("Directional Light");
    ImGui::SliderFloat3("Direction", dir_direction, -1.0f, 1.0f);
    ImGui::SliderFloat3("Ambient##dir", dir_ambient, -1.0f, 1.0f);
    ImGui::SliderFloat3("Diffuse##dir", dir_diffuse, -1.0f, 1.0f);
    ImGui::SliderFloat3("Specular##dir", dir_specular, -1.0f, 1.0f);

    ImGui::Spacing();
    ImGui::Text("Point Lights");
    ImGui::SliderFloat3("Ambient##point", point_ambient, -1.0f, 1.0f);
    ImGui::SliderFloat3("Diffuse##point", point_diffuse, -1.0f, 1.0f);
    ImGui::SliderFloat3("Specular##point", point_specular, -1.0f, 1.0f);
    for (int i = 0; i < 3; i++) {
        std::string label = "Torch " + std::to_string(i);

        if (ImGui::TreeNode(label.c_str())) {

            ImGui::Checkbox("Enabled", &point_enabled[i]);

            ImGui::TreePop();
        }
    }
    ImGui::Spacing();
    ImGui::Text("Post Processing");
    ImGui::Checkbox("MSAA", &msaa_enabled);

    ImGui::End();

    graphics->end_gui();
}

void GUIController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_F).state() == engine::platform::Key::State::JustPressed) {
        set_enable(!is_enabled());
    }
}
}// namespace app
