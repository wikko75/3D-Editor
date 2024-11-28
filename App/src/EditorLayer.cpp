#include "EditorLayer.hpp"
#include "GL/glew.h"
#include "imgui.h"  
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ImGuiFileDialog.h"
#include <filesystem>
#include <fstream>
#include "App.hpp"


EditorLayer::EditorLayer(Window* window, const std::string& name)
: Layer{name}
{
    m_renderer    = std::make_unique<Renderer>(window->getWindow());
    m_framebuffer = std::make_shared<FrameBuffer>(window->getWidth(), window->getHeight());

    m_camera = std::make_shared<Camera>(window->getWindow(), glm::vec3{0.0f, 0.0f, 3.0f});
    m_viewport_size = {window->getWidth(), window->getHeight()};

    // temp
    // addSquare(0.5f);
    // addSquare(0.3f);
    // addSquare(0.2f);
}


// auto EditorLayer::beginScene(std::shared_ptr<Camera> camera) -> void
// {
//     m_camera = camera;
//     //calculate stuff related to camera
// }

auto EditorLayer::addSquare(const float size, const glm::vec4& color) -> void
{
    if (size <= 0.1)
        return;

    const float halfSize = size / 2.0f;

    std::vector<Vertex> vertices = {
        {{-halfSize, -halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, color},
        {{ halfSize, -halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, color},
        {{ halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, color},
        {{ halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, color},
        {{-halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, color},
        {{-halfSize, -halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, color},

        {{-halfSize, -halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, color},
        {{ halfSize, -halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, color},
        {{ halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, color},
        {{ halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, color},
        {{-halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, color},
        {{-halfSize, -halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, color},

        {{-halfSize, -halfSize, -halfSize}, {-1.0f, 0.0f, 0.0f}, color},
        {{-halfSize, -halfSize,  halfSize}, {-1.0f, 0.0f, 0.0f}, color},
        {{-halfSize,  halfSize,  halfSize}, {-1.0f, 0.0f, 0.0f}, color},
        {{-halfSize,  halfSize,  halfSize}, {-1.0f, 0.0f, 0.0f}, color},
        {{-halfSize,  halfSize, -halfSize}, {-1.0f, 0.0f, 0.0f}, color},
        {{-halfSize, -halfSize, -halfSize}, {-1.0f, 0.0f, 0.0f}, color},

        {{ halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, color},
        {{ halfSize, -halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, color},
        {{ halfSize,  halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, color},
        {{ halfSize,  halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, color},
        {{ halfSize,  halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, color},
        {{ halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, color},

        {{-halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, color},
        {{-halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, color},
        {{ halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, color},
        {{ halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, color},
        {{ halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, color},
        {{-halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, color},

        {{-halfSize, -halfSize, -halfSize}, {0.0f, -1.0f, 0.0f}, color},
        {{-halfSize, -halfSize,  halfSize}, {0.0f, -1.0f, 0.0f}, color},
        {{ halfSize, -halfSize,  halfSize}, {0.0f, -1.0f, 0.0f}, color},
        {{ halfSize, -halfSize,  halfSize}, {0.0f, -1.0f, 0.0f}, color},
        {{ halfSize, -halfSize, -halfSize}, {0.0f, -1.0f, 0.0f}, color},
        {{-halfSize, -halfSize, -halfSize}, {0.0f, -1.0f, 0.0f}, color},
    };

    auto shader = std::make_shared<Shader>(
            std::filesystem::current_path() / "App"  / "assets" / "shaders" / "basic_vertex.glsl",
            std::filesystem::current_path() / "App" / "assets" / "shaders" / "basic_fragment.glsl"
    );

    std::shared_ptr<Mesh> mesh { std::make_shared<Mesh>(vertices, shader)};
    m_meshes.push_back(mesh);
}

void EditorLayer::onUpdate(float delta_time)
{
    m_camera->update(delta_time);

    m_renderer->clear({0.2, 0.2, 0.2, 1});  

    m_framebuffer->bind();
    {
        m_renderer->clear({0.2, 0.2, 0.2, 1});   
        
        for (const auto& mesh : m_meshes)
        {
            mesh->recalculateModelMatrix();
            auto* shader { mesh->getShader() };
            shader->setUniformMatrix4f("u_view_projection_mtx", false, glm::value_ptr(m_camera->getViewProjectionMatrix()));
            shader->setUniformMatrix4f("u_model_mtx", false, glm::value_ptr(mesh->getModelMatrix()));

            shader->setUniform3f("u_color", 1.0, 0.5, 0.2);
            shader->setUniformi("u_primitive_type", 0);
            mesh->setRenderMode(GL_TRIANGLES);
            // Logger::LOG(" onUpdate in EL", Type::DEBUG);
            m_renderer->render(mesh);
            
            if (m_edit_mode == EditMode::VERTEX && mesh == m_selected_mesh)
            {
                fmt::print("Selected: {}, Current: {}\n",fmt::ptr(m_selected_mesh.get()), fmt::ptr(mesh.get()));
                shader->setUniformi("u_primitive_type", 1);
                mesh->setRenderMode(GL_POINTS);
                m_renderer->render(mesh);
            }
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

        if (m_selected_mesh && m_edit_mode == EditMode::VERTEX)
        {
            dispatcher.dispatch<MouseButtonPressedEvent>([this](Event& event)
            {   
                Logger::LOG("Vertex Edit Mode |  MousePressed!", Type::WARNING);
                MouseButtonPressedEvent& mouse_event {static_cast<MouseButtonPressedEvent&>(event)};
               
                // left mouse button
                if (mouse_event.getMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
                {   
                   m_selected_mesh->selectVertexAtPosition(m_viewport_mouse_pos_model);
                   fmt::print("Exited selectVertexAtPosition()\n");
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
                
                if (m_selected_mesh)
                {
                    glm::vec4 screen_pos_with_depth = glm::vec4(ndc_x, ndc_y, 2.0f * depth - 1.0f, 1.0f);
                    glm::vec4 world_pos_with_depth = glm::inverse(m_camera->getViewProjectionMatrix() * m_selected_mesh->getModelMatrix()) * screen_pos_with_depth;
                    world_pos_with_depth /= world_pos_with_depth.w;

                    m_viewport_mouse_pos_model = {world_pos_with_depth.x, world_pos_with_depth.y, world_pos_with_depth.z};
                    ImGui::Text("Mouse position (Model): (%.1f, %.1f, %.1f)", m_viewport_mouse_pos_model.x, m_viewport_mouse_pos_model.y, m_viewport_mouse_pos_model.z);
                }
                
                ImGui::Text("Depth: (%.3f)", depth);
                
                if (m_edit_mode == EditMode::VERTEX && m_selected_mesh)
                {
                    ImGui::Separator();
                    ImGui::Text("Selected Vertices: %d", m_selected_mesh->getSelectedVerticesCount());
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


    if (ImGui::Begin("Meshes"))
    {
        //  add some separator
        if (ImGui::CollapsingHeader("Cube"))
        {
            static float s_square_size {0.0f};
            static glm::vec4 s_argb_color {0.8f, 0.8f, 0.8, 1.0f};

            ImGui::InputFloat("Size", &s_square_size, 0.1f);
            ImGui::Separator();
            ImGui::ColorPicker4("Color", &s_argb_color.x, (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueWheel));
            
            if (ImGui::Button("create"))
            {                
                addSquare(s_square_size, s_argb_color);
            }

        }

        // if (ImGui::CollapsingHeader("Triangle"))
        // {
        //     static float s_triangle_size {0.0f};

        //     ImGui::InputFloat("Size:", &s_triangle_size, 0.1f);

        //     if (ImGui::Button("create"))
        //     {                
        //         // create square
        //     }
        // }
        ImGui::Text("Meshes:");
        ImGui::Columns(1);
        static size_t selected_index {0};
        for (size_t i = 0; i < m_meshes.size(); ++i) 
        {
            if (ImGui::Selectable(std::string("Mesh: ").append(std::to_string(i)).c_str(), selected_index == i)) 
            {
                selected_index = i;
                fmt::print("Selected: {}\n", i);
                
                if (!m_meshes[i]) {
                    fmt::print("Mesh at index {} is nullptr\n", i);
                    continue;
                }

                if (m_meshes[i] == m_selected_mesh) 
                {
                    fmt::print("This mesh is already selected!\n");
                }


                // Debugowanie, czy m_selected_mesh jest poprawnie ustawiany
                m_selected_mesh = m_meshes[i];
                fmt::print("Current mesh pointer in SELECTABLE: {}\n", fmt::ptr(m_meshes[i].get()));

                // Debugowanie, czy m_selected_mesh wskazuje na odpowiedni obiekt
                fmt::print("Selected mesh pointer: {}\n", fmt::ptr(m_selected_mesh.get()));
            }
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

        static Mesh::Transformation s_transformation {};

        if (m_selected_mesh)
        {
            s_transformation = m_selected_mesh->getTransform();
        }

        if (ImGui::CollapsingHeader("Transform"))
        {
            ImGui::SeparatorText("Translate");
            {
                for (int i {0}; i < 3; ++i)
                {
                    ImGui::PushID(i);
                    ImGui::DragFloat("##position", &s_transformation.position[i], 0.005f);
                    ImGui::SameLine(); ImGui::TextColored(colors[i], axis[i]);
                    ImGui::PopID();
                }
            }
            ImGui::SeparatorText("Rotate");
            {   
                for (int i {0}; i < 3; ++i)
                {
                    ImGui::PushID(i);
                    ImGui::DragFloat("##rotation", &s_transformation.rotation[i], 5.f, -180.f, 180.f);
                    ImGui::SameLine(); ImGui::TextColored(colors[i], axis[i]);
                    ImGui::PopID();
                }

            }
            ImGui::SeparatorText("Scale");
            {
                for (int i {0}; i < 3; ++i)
                {
                    ImGui::PushID(i);
                    ImGui::DragFloat("##scale", &s_transformation.scale[i], 0.005f);
                    ImGui::SameLine(); ImGui::TextColored(colors[i], axis[i]);
                    ImGui::PopID();
                }
            }

            ImGui::Dummy({0.f, 5.f});
            if (ImGui::Button("Back to default"))
            {
                s_transformation = Mesh::Transformation {};
            }
        }

        if (m_selected_mesh)
        {
            // ugly, needs refactor
            m_selected_mesh->getTransform() = s_transformation;
        }

        static int selected_mode = 0;

        if (ImGui::CollapsingHeader("Edit"))
        {
            ImGui::TextWrapped("To edit a mesh, first select it from Meshes panel");
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
                            if (m_selected_mesh)
                            {
                                m_selected_mesh->updateSelectedVertices(offset);
                            }
                        }
                        ImGui::SameLine(); ImGui::TextColored(colors[i], axis[i]);
                        ImGui::PopID();
                    }

                    if (ImGui::Button("Deselect All"))
                    {
                        if (m_selected_mesh)
                        {
                            m_selected_mesh->deselectAllVertices();
                        }
                    }
                }


                if (ImGui::CollapsingHeader("Add"))
                {
                    ImGui::TextWrapped("To add a vertex, first select a mesh, then choose 2 vertices of a mesh. The new vertex will be created in between the two selected vertices");

                    static bool s_select_vertices = false;
                    static bool s_can_add = false;
                    
                    ImGui::Checkbox("Select Vertices", &s_select_vertices);
                    
                    if (s_select_vertices && m_selected_mesh)
                    {
                        if (ImGui::BeginTable("Selected Vertices", 1, ImGuiTableFlags_Borders))
                        {
                            ImGui::TableNextColumn();
                            ImGui::Text("Vertex Position");

                            if (m_selected_mesh->getSelectedVerticesCount() > 2)
                            {
                                ImGui::Text("Only 2 vertices migh be selected!");
                                s_can_add = false;
                            }
                            else
                            {
                                for (const auto& vertex : m_selected_mesh->getSelectedVertices())
                                {
                                    glm::vec3 position {vertex.getPosition()};
                                    ImGui::TableNextColumn();
                                    ImGui::Text("(%.3f, %.3f, %.3f)", position.x, position.y, position.z);
                                }

                                if (m_selected_mesh->getSelectedVerticesCount() == 2)
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
                            const auto& v {m_selected_mesh->getSelectedVertices()};
                            m_selected_mesh->addVertex(v[0], v[1]);
                        }
                        ImGui::EndDisabled();
                        ImGui::SameLine();
                        if (ImGui::Button("Deselect All##vertices_button"))
                        {
                            Logger::LOG("DISELECTING", Type::ERROR);
                            m_selected_mesh->deselectAllVertices();
                        }
                    }
                }

                if (ImGui::CollapsingHeader("Color"))
                {
                    static glm::vec4 s_color {0.8f, 0.8f, 0.8f, 1.0f};
                    ImGui::ColorPicker4("Color", &s_color.x, ImGuiColorEditFlags_DisplayRGB);
                    fmt::print("New color: {}, {}, {}\n", s_color.x, s_color.y, s_color.z);

                    ImGui::Dummy({0.0f, 1.0f});

                    ImGui::TextWrapped("To change color of a vertex, first select at least one vertex");
                    if (ImGui::Button("Change color"))
                    {
                        if (m_selected_mesh)
                        {
                            m_selected_mesh->updateSelectedVertices(s_color);
                        }
                    }
                }
            }
        }
        ImGui::End();
    }
}



auto EditorLayer::openFileDialog(bool& show_saving_dialog, const DialogType type) ->  void
{
    IGFD::FileDialogConfig config;
    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".ini", config);

    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) 
    {
        if (ImGuiFileDialog::Instance()->IsOk()) 
        { 
            const std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

            if (type == DialogType::SAVING)
            {

                fmt::print("Saving file: {}\n", filePathName);
            }
            else if (type == DialogType::LOADING)
            {
                fmt::print("Loading file: {}\n", filePathName);
            }
        }
    
        show_saving_dialog = false;
        ImGuiFileDialog::Instance()->Close();
    }
}

auto EditorLayer::showMenuBar() -> void
{
    static bool s_show_saving_dialog {false};
    static DialogType s_dialog_type {DialogType::LOADING};

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save..."))
            {
                s_show_saving_dialog = true;
                s_dialog_type = DialogType::SAVING;
                fmt::print("Saving...\n");
            }
            if (ImGui::MenuItem("Load..."))
            {
                s_show_saving_dialog = true;
                s_dialog_type = DialogType::LOADING;
            }
            if (ImGui::MenuItem("Exit"))
            {
                App::get()->onClose();
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::BeginMenu("Renderer"))
            {
                static bool m_wire_frame_mode_enabled {false};
                if (ImGui::MenuItem("Wireframe Mode",nullptr, &m_wire_frame_mode_enabled))
                {
                    if (m_wire_frame_mode_enabled)
                    {
                        m_renderer->enableWireframeMode();
                    }
                    else
                    {
                        m_renderer->disableWireframeMode();
                    }
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (s_show_saving_dialog)
        {
            openFileDialog(s_show_saving_dialog, s_dialog_type);
        }
        ImGui::EndMainMenuBar();
    }
}
