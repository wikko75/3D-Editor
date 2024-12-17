#ifndef _EDITOR_LAYER_
#define _EDITOR_LAYER_

#include "LayersSystem.hpp"

#include <memory>
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "FrameBuffer.hpp"


enum class EditMode
{
    VERTEX,
    FACE,
    NONE
};


enum class DialogType
{
    LOADING,
    SAVING
};



class  EditorLayer : public Layer
{
public:
    EditorLayer(Window* window, const std::string& name = "Renderer");

    auto addSquare(const float size, const glm::vec4& color = {8.0f, 8.0f, 8.0f, 1.0f}) -> void;

    auto addTriangle() -> void;
    
    auto addPointLight(const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}) -> void;
    
    void onUpdate(float delta_time) override;

    void onImGuiRender() override;

    void onEvent(Event& event) override;

    ~ EditorLayer() = default;

private:
    auto showMenuBar() -> void;

    auto openFileDialog(bool& show_saving_dialog,  const DialogType type) -> void;

    auto saveEditState(std::string_view path) const-> bool;

    auto loadEditState(std::string_view path) -> bool;

private:
    std::unique_ptr<Renderer> m_renderer;
    std::shared_ptr<Camera>   m_camera;
    std::shared_ptr<Mesh>     m_selected_mesh;
    std::shared_ptr<FrameBuffer> m_framebuffer;
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::shared_ptr<Mesh> m_light_mesh;

    std::pair<int, int> m_viewport_size;
    std::pair<float, float> m_viewport_mouse_pos;
    glm::vec3 m_viewport_mouse_pos_model;
    bool m_viewport_active = false;
    
    EditMode m_edit_mode {EditMode::NONE};
    
};

#endif