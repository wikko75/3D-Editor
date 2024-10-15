#ifndef _LAYERS_HPP_
#define _LAYERS_HPP_

#include "Logger.hpp"
#include <string>
#include <vector>
#include "Events.hpp"


class Layer
{
public:
    Layer(const std::string& name)
    : m_is_active {true}
    , m_name      {name}
    {
        Logger::LOG("Layer " + name + " created", Type::INFO);
    }

    virtual auto onUpdate() -> void
    {}

    virtual auto onEvent(Event& event) -> void
    {}

    virtual ~Layer() = default;

protected:
    bool m_is_active;
    std::string m_name;
};


class LayerVector 
{
public:
    LayerVector()
    : m_layers {}
    {}

    auto push(Layer* layer) -> void
    {
        m_layers.push_back(layer);
    }

    auto pop(Layer* layer) -> void
    {
        auto to_delete { std::find(std::begin(m_layers), std::end(m_layers), layer) };
        if (to_delete != std::end(m_layers))
        {
            m_layers.erase(to_delete);
        }
    }

    auto begin() -> std::vector<Layer *>::iterator
    {
        return m_layers.begin();
    }

    auto end() -> std::vector<Layer *>::iterator
    {
        return m_layers.end();
    }

    ~LayerVector()
    {
        for (auto* layer : m_layers)
        {
            delete layer;
        }
    }

private:
    std::vector<Layer*> m_layers;
};


#endif
