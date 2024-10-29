#ifndef _FRAME_BUFFER_HPP
#define _FRAME_BUFFER_HPP

#include "GL/glew.h"
#include "Logger.hpp"
#include <utility>


class FrameBuffer
{
public:
    FrameBuffer(int width, int height)
    : m_id{}
    , m_color_attachment {}
    , m_depth_attachment {}
    {
        if (width < 1|| height < 1)
        {
            Logger::LOG("Improper Framebuffer size", Type::ERROR);
            return;
        }

        m_width = width;
        m_height = height;

        glDeleteTextures(1, &m_color_attachment);
        glDeleteTextures(1, &m_depth_attachment);

        create();
    }

    auto bind() -> void
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    }

    auto unbind() -> void
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    auto getColorAttachment() -> unsigned int 
    {
        return m_color_attachment;
    }

    auto getDepthAttachment() -> unsigned int 
    {
        return m_depth_attachment;
    }

    auto getSize() -> std::pair<int, int>
    {
        return { m_width, m_height };
    }

    auto resize(const std::pair<int,int>& size) -> void
    {
        m_width  = size.first;
        m_height = size.second;


        if (m_width < 1|| m_height < 1)
        {
            Logger::LOG("Improper Framebuffer size", Type::ERROR);
            return;
        }

        create();
    }

    ~FrameBuffer()
    {
        glDeleteFramebuffers(1, &m_id);
        glDeleteTextures(1, &m_color_attachment);
        glDeleteTextures(1, &m_depth_attachment);
    }

private:
    auto create() -> void
    {
        if (m_id == 0)
            glCreateFramebuffers(1, &m_id);

        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        //  color
        glGenTextures(1, &m_color_attachment);
        glBindTexture(GL_TEXTURE_2D, m_color_attachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment, 0);  


        // depth and stencil
        glGenTextures(1, &m_depth_attachment);
        glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH24_STENCIL8, m_depth_attachment, 0);
       
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Logger::LOG("Error while creating FrameBuffer", Type::ERROR);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return;
        }
        Logger::LOG("Framebuffer created", Type::DEBUG);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

private:
    unsigned int m_id;
    unsigned int m_color_attachment;
    unsigned int m_depth_attachment;
    int m_width;
    int m_height;
};



#endif