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


class  EditorLayer : public Layer
{
public:
    EditorLayer(Window* window, const std::string& name = "Renderer");

    // auto beginScene(std::shared_ptr<Camera> camera) -> void;

    auto addDrawable(std::shared_ptr<VertexArray> vao, std::shared_ptr<Shader> shader) -> void;

    auto addMesh(std::shared_ptr<Mesh>& mesh) -> void;

    void onUpdate(float delta_time) override;

    void onImGuiRender() override;

    void onEvent(Event& event) override;

    ~ EditorLayer() = default;

private:
    auto showMenuBar() -> void;

private:
    std::unique_ptr<Renderer> m_renderer;
    std::shared_ptr<Camera>   m_camera;
    std::shared_ptr<Mesh>     m_mesh;
    std::shared_ptr<FrameBuffer> m_framebuffer;

    std::pair<int, int> m_viewport_size;
    std::pair<float, float> m_viewport_mouse_pos;
    bool m_viewport_active = false;
    
    EditMode m_edit_mode {EditMode::NONE};
    
};

#endif