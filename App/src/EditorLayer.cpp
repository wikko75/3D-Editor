#include "EditorLayer.hpp"
#include "GL/glew.h"
#include "imgui.h"  
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "App.hpp"


EditorLayer::EditorLayer(Window* window, const std::string& name)
: Layer{name}
{
    m_renderer    = std::make_unique<Renderer>(window->getWindow());
    m_framebuffer = std::make_shared<FrameBuffer>(window->getWidth(), window->getHeight());

    m_camera = std::make_shared<Camera>(window->getWindow(), glm::vec3{0.0f, 0.0f, 3.0f});
    m_viewport_size = {window->getWidth(), window->getHeight()};
}


// auto EditorLayer::beginScene(std::shared_ptr<Camera> camera) -> void
// {
//     m_camera = camera;
//     //calculate stuff related to camera
// }


auto EditorLayer::addDrawable(std::shared_ptr<VertexArray> vao, std::shared_ptr<Shader> shader) -> void  // change for mesh
{
    m_renderer->submit({vao, shader});
}


auto EditorLayer::addMesh(std::shared_ptr<Mesh>& mesh) -> void
{
    m_mesh = mesh;
}


void EditorLayer::onUpdate(float delta_time)
{
    m_camera->update(delta_time);

    Logger::LOG("Layer | Renderer | onUpdate()", Type::DEBUG);

    m_mesh->recalculateModelMatrix();
    auto* shader { m_mesh->getShader() };
    shader->setUniformMatrix4f("u_view_projection_mtx", false, glm::value_ptr(m_camera->getViewProjectionMatrix()));
    shader->setUniformMatrix4f("u_model_mtx", false, glm::value_ptr(m_mesh->getModelMatrix()));
    
    m_renderer->clear({0.2, 0.2, 0.2, 1});   

    // render to my framebuffer
    m_framebuffer->bind();
    
    {
        m_renderer->clear({0.2, 0.2, 0.2, 1});   

        shader->setUniform3f("u_color", 1.0, 0.5, 0.2);
        shader->setUniformi("u_primitive_type", 0);
        m_mesh->setRenderMode(GL_TRIANGLES);
        m_renderer->render(m_mesh);
        
        if (m_edit_mode == EditMode::VERTEX)
        {
            shader->setUniformi("u_primitive_type", 1);
            m_mesh->setRenderMode(GL_POINTS);
            m_renderer->render(m_mesh);
        }
    }
   
    m_framebuffer->unbind();
}


void EditorLayer::onEvent(Event& event)
{
    // Logger::LOG("Layer | EditorLayer |  onEvent()", Type::DEBUG);
    
    EventDispacher dispatcher {event};
    
    if (m_viewport_active)
    {
        m_camera->onEvent(event);

        if (m_edit_mode == EditMode::VERTEX)
        {
            dispatcher.dispatch<MouseButtonPressedEvent>([this](Event& event)
            {   
                Logger::LOG("Vertex Edit Mode |  MousePressed!", Type::WARNING);
                MouseButtonPressedEvent& mouse_event {static_cast<MouseButtonPressedEvent&>(event)};
               
                // left mouse button
                if (mouse_event.getMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
                {   
                   m_mesh->selectVertexAtPosition(m_viewport_mouse_pos_model);
                }

                return true;
            });
        }
        
    }

    dispatcher.dispatch<WindowResizeEvent>([](Event& event)
    {
        // auto& e {static_cast<WindowResizeEvent&>(event)};
        // glViewport(0, 0, e.getWidth(), e.getHeight());
        return false;
    });

    dispatcher.dispatch<MouseMovedEvent>([this](Event& event)
    {
        float xpos {static_cast<MouseMovedEvent&>(event).getX()};
        float ypos {static_cast<MouseMovedEvent&>(event).getY()};

        m_camera->onMouseMove(xpos, ypos);
        // temp
        return false;
    });
}


void EditorLayer::onImGuiRender()
{
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    showMenuBar();
    ImGui::ShowDemoWindow();
    

    if (ImGui::Begin("Viewport"))
    {
        float viewportSizeWidth  {ImGui::GetContentRegionAvail().x};
        float viewportSizeHeight {ImGui::GetContentRegionAvail().y};

        ImGui::Image((void*)m_framebuffer->getColorAttachment(), { m_viewport_size.first, m_viewport_size.second }, {0, 1}, {1, 0});
        
        if (viewportSizeWidth != m_viewport_size.first || viewportSizeHeight != m_viewport_size.second)
        {
            // adjust framebuffer
            m_viewport_size = {viewportSizeWidth, viewportSizeHeight};
            glViewport(0, 0, viewportSizeWidth, viewportSizeHeight);
            m_framebuffer->resize(m_viewport_size);

            m_camera->setAspectRatio(viewportSizeWidth / viewportSizeHeight);
        }



        if (ImGui::IsWindowHovered() && ImGui::IsWindowFocused())
        {   
            m_viewport_active = true;

            auto [x_screen_pos, y_screen_pos] {ImGui::GetCursorScreenPos()};
            auto [x_mouse_pos, y_mouse_pos]   {ImGui::GetMousePos()};

            m_viewport_mouse_pos = {x_mouse_pos - x_screen_pos, viewportSizeHeight - (y_screen_pos - y_mouse_pos)};  //viewportSizeHeight - because ImGui y axis is up

            float ndc_x = (2.0f * m_viewport_mouse_pos.first) / viewportSizeWidth - 1.0f;
            float ndc_y = 1.0f - (2.0f * m_viewport_mouse_pos.second) / viewportSizeHeight;
            
            //  get depth information
            m_framebuffer->bind();
            float depth;
            glReadPixels(m_viewport_mouse_pos.first, y_screen_pos - y_mouse_pos, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
            m_framebuffer->unbind();

            glm::vec4 screen_pos_with_depth = glm::vec4(ndc_x, ndc_y, 2.0f * depth - 1.0f, 1.0f);
            glm::vec4 world_pos_with_depth = glm::inverse(m_camera->getViewProjectionMatrix() * m_mesh->getModelMatrix()) * screen_pos_with_depth;
            world_pos_with_depth /= world_pos_with_depth.w;

            m_viewport_mouse_pos_model = {world_pos_with_depth.x, world_pos_with_depth.y, world_pos_with_depth.z};
            
            // Logger::LOG("DEPTH: " + std::to_string(depth), Type::ERROR);
            
            // stats overlay
            ImGui::SetNextWindowBgAlpha(0.35f);
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            ImGui::SetNextWindowPos({ImGui::GetWindowPos().x + 20, ImGui::GetWindowPos().y + 50});
            
            if (ImGui::Begin("ViewportStats",nullptr, window_flags))
            {   
                ImGui::Text("Viewport stats.");
                ImGui::Separator();
                ImGui::Text("Size: (%d,%d)", m_viewport_size.first, m_viewport_size.second);
                ImGui::Text("Mouse position (Viewport): (%.1f,%.1f)", m_viewport_mouse_pos.first, m_viewport_mouse_pos.second);
                ImGui::Text("Mouse position (Model): (%.1f, %.1f, %.1f)", m_viewport_mouse_pos_model.x, m_viewport_mouse_pos_model.y, m_viewport_mouse_pos_model.z);
                ImGui::Text("Depth: (%.3f)", depth);
                
                if (m_edit_mode == EditMode::VERTEX)
                {
                    ImGui::Separator();
                    ImGui::Text("Selected Vertices: %d", m_mesh->getSelectedVerticesCount());
                }

                ImGui::End();
            }
        }
        else
        {
            m_viewport_active = false;
        }

        ImGui::End();   
    }


    if(ImGui::Begin("Settings"))
    {
        static const ImVec4 colors[3] = {
            ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
            ImVec4(0.0f, 1.0f, 0.0f, 1.0f),
            ImVec4(0.0f, 0.0f, 1.0f, 1.0f)
        };

        static const char* axis[3] = {
            "X",
            "Y",
            "Z"
        };

        if (ImGui::CollapsingHeader("Transform"))
        {
            
            ImGui::SeparatorText("Translate");
            {
                for (int i {0}; i < 3; ++i)
                {
                    ImGui::PushID(i);
                    ImGui::DragFloat("##position", &m_mesh->getTransform().position[i], 0.005f);
                    ImGui::SameLine(); ImGui::TextColored(colors[i], axis[i]);
                    ImGui::PopID();
                }
            }
            ImGui::SeparatorText("Rotate");
            {   
                for (int i {0}; i < 3; ++i)
                {
                    ImGui::PushID(i);
                    ImGui::DragFloat("##rotation", &m_mesh->getTransform().rotation[i], 5.f, -180.f, 180.f);
                    ImGui::SameLine(); ImGui::TextColored(colors[i], axis[i]);
                    ImGui::PopID();
                }

            }
            ImGui::SeparatorText("Scale");
            {
                for (int i {0}; i < 3; ++i)
                {
                    ImGui::PushID(i);
                    ImGui::DragFloat("##scale", &m_mesh->getTransform().scale[i], 0.005f);
                    ImGui::SameLine(); ImGui::TextColored(colors[i], axis[i]);
                    ImGui::PopID();
                }
            }

            ImGui::Dummy({0.f, 5.f});
            if (ImGui::Button("Back to default"))
            {
                m_mesh->getTransform() = Mesh::Transformation {};
            }
        }

        static int selected_mode = 0;

        if (ImGui::CollapsingHeader("Edit"))
        {
            if (ImGui::RadioButton("Vertex Mode", selected_mode == 1))
            {
                selected_mode = 1;
                m_edit_mode = EditMode::VERTEX;
            }

            if (ImGui::RadioButton("Face Mode", selected_mode == 2))
            {
                selected_mode = 2;
                m_edit_mode = EditMode::FACE;
            }

            if(ImGui::RadioButton("None", selected_mode == 0))
            {
                selected_mode = 0;
                m_edit_mode = EditMode::NONE;
            }


            if (m_edit_mode == EditMode::VERTEX)
            {
                glm::vec3 offset = {0.0f, 0.0f, 0.0f};

                if(ImGui::CollapsingHeader("Translate"))
                {
                    for (int i {0}; i < 3; ++i)
                    {
                        ImGui::PushID(i);
                        if(ImGui::DragFloat("##VertexPosition", &offset[i], 0.005f, 0.0f))
                        {
                            m_mesh->updateSelectedVertices(offset);
                        }
                        ImGui::SameLine(); ImGui::TextColored(colors[i], axis[i]);
                        ImGui::PopID();
                    }

                    if (ImGui::Button("Deselect All"))
                    {
                        m_mesh->deselectAllVertices();
                    }
                }


                if (ImGui::CollapsingHeader("Add"))
                {
                    ImGui::TextWrapped("To add a vertex, choose 2 vertices of a mesh. The new vertex will be created in between the two selected vertices");

                    static bool s_select_vertices = false;
                    static bool s_can_add = false;
                    ImGui::Checkbox("Select Vertices", &s_select_vertices);
                    if (s_select_vertices)
                    {
                        if (ImGui::BeginTable("Selected Vertices", 1, ImGuiTableFlags_Borders))
                        {
                            ImGui::TableNextColumn();
                            ImGui::Text("Vertex Position");

                            if (m_mesh->getSelectedVerticesCount() > 2)
                            {
                                ImGui::Text("Only 2 vertices migh be selected!");
                                s_can_add = false;
                            }
                            else
                            {
                                for (const auto& vertex : m_mesh->getSelectedVertices())
                                {
                                    glm::vec3 position {vertex.getPosition()};
                                    ImGui::TableNextColumn();
                                    ImGui::Text("(%.3f, %.3f, %.3f)", position.x, position.y, position.z);
                                }

                                if (m_mesh->getSelectedVerticesCount() == 2)
                                {
                                    s_can_add = true;
                                }
                                else
                                {
                                    s_can_add = false;
                                }
                            }

                            ImGui::EndTable();
                        }


                        ImGui::BeginDisabled(!s_can_add);
                        if (ImGui::Button("Add##vertex_button"))
                        {
                            const auto& v {m_mesh->getSelectedVertices()};
                            m_mesh->addVertex(v[0], v[1]);
                        }
                        ImGui::EndDisabled();
                        ImGui::SameLine();
                        if (ImGui::Button("Deselect All##vertices_button"))
                        {
                            Logger::LOG("DISELECTING", Type::ERROR);
                            m_mesh->deselectAllVertices();
                        }
                    }
                    
                }
               

            }
        }
        ImGui::End();
    }
}


auto EditorLayer::showMenuBar() -> void
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                App::get()->onClose();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
