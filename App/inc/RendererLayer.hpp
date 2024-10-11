#ifndef _RENDERER_LAYER_
#define _RENDERER_LAYER_

#include "LayersSystem.hpp"

class  RendererLayer : public Layer
{
public:
    RendererLayer(const std::string& name = "Renderer")
    : Layer{name}
    {

    }

    void onUpdate() override
    {
        // rendering stuff
        fmt::println("Layer | Renderer | onUpdate()");
    } 


    void onEvent(Event& event) override
    {
        // capture events
        fmt::println("Layer | Renderer |  onEvent()");
    }
    ~ RendererLayer() = default;
};


#endif