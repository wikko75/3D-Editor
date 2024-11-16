#include "App.hpp"


App* App::s_instance = nullptr;


App::App()
: m_window {}
{        
    if (s_instance)
    {
        Logger::LOG("App already running", Type::ERROR);
        return;
    }

    s_instance = this;

    m_window = std::make_unique<Window>();    
    m_imgui_layer  = std::make_unique<ImGuiLayer>(m_window.get());
    m_editor_layer = std::make_unique<EditorLayer>(m_window.get());

    m_window->setEventCallback(std::bind(&App::onEvent, this, std::placeholders::_1));
}

auto App::run() -> void
{
    std::vector<Vertex> vertices
    {   
        Vertex{{-0.7f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        Vertex{{ 0.0f, 0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        Vertex{{0.7f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    };

    // std::vector<unsigned int> indices {0, 1, 2};

    auto shader = std::make_shared<Shader>(
            std::filesystem::current_path() / "App"  / "assets" / "shaders" / "basic_vertex.glsl",
            std::filesystem::current_path() / "App" / "assets" / "shaders" / "basic_fragment.glsl"
    );

    // sample mesh
    std::shared_ptr<Mesh> mesh { std::make_shared<Mesh>(vertices,  shader)};

    double prev_frame {glfwGetTime()};
    double curr_frame {};
    double delta_time {};

    while (!m_should_exit)
    {
        curr_frame = glfwGetTime();
        delta_time = curr_frame - prev_frame;
        prev_frame = curr_frame; 

        m_editor_layer->addMesh(mesh);
        
        m_editor_layer->onUpdate(delta_time);
        
        // render UI
        {
            m_imgui_layer->beginFrame();

            m_editor_layer->onImGuiRender();

            m_imgui_layer->endFrame();
        }

        m_window->onUpdate();
    }
}


auto App::onEvent(Event& event) -> void
{
    
    EventDispacher dispatcher {event};
    dispatcher.dispatch<WindowResizeEvent>([](Event& event){
        fmt::print("Window has been resized!\n");
        return false;
    });


    dispatcher.dispatch<WindowCloseEvent>([this](Event& event){
        fmt::println("WINDOW_CLOSE\n");
        onClose();
        return true;
    });

    m_editor_layer->onEvent(event);
    m_imgui_layer->onEvent(event);
}


auto App::onClose() -> void
{
    m_should_exit = true;
}


auto App::get() -> App *
{
    return s_instance;
}
