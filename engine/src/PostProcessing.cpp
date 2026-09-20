//
// Created by andrija on 18. 9. 2026..
//

#include <glad/glad.h>
#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/PostProcessing.hpp>
#include <engine/resources/Shader.hpp>
#include <stdexcept>

namespace engine::resources {
    PostProcessing::PostProcessing(int width, int height) : m_width{width}
                                                        , m_height{height} {
        create_framebuffer();

        const float quad_vertices[] = {
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
        };

        CHECKED_GL_CALL(glGenVertexArrays, 1, &m_vao);
        CHECKED_GL_CALL(glGenBuffers, 1, &m_vbo);

        CHECKED_GL_CALL(glBindVertexArray, m_vao);
        CHECKED_GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_vbo);

        CHECKED_GL_CALL(glVertexAttribPointer, 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        CHECKED_GL_CALL(glEnableVertexAttribArray, m_vao);
        CHECKED_GL_CALL(glVertexAttribPointer, 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        reinterpret_cast<void *>(2 * sizeof(float)));
        CHECKED_GL_CALL(glEnableVertexAttribArray, 1);
        CHECKED_GL_CALL(glBindVertexArray, 0);
    }

    PostProcessing::~PostProcessing() {
        destroy();
    };

    void PostProcessing::create_framebuffer() {
        CHECKED_GL_CALL(glGenFramebuffers, 1, &m_fbo);
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_fbo);

        CHECKED_GL_CALL(glGenTextures, 1, &m_color_texture);
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_color_texture);

        CHECKED_GL_CALL(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                        nullptr);

        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        CHECKED_GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_texture,
                        0);

        CHECKED_GL_CALL(glGenRenderbuffers, GL_RENDERBUFFER, &m_depth_stencil_rbo);
        CHECKED_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, m_depth_stencil_rbo);
        CHECKED_GL_CALL(glRenderbufferStorage, GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);

        CHECKED_GL_CALL(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                        m_depth_stencil_rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Framebuffer not complete");
        }

        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    }

    void PostProcessing::begin() {
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_fbo);

        CHECKED_GL_CALL(glViewport, 0, 0, m_width, m_height);

        CHECKED_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void PostProcessing::end(Shader *shader) {
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

        CHECKED_GL_CALL(glDisable, GL_DEPTH_TEST);

        shader->use();
        shader->set_int("screen_texture", 0);

        CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_color_texture);

        CHECKED_GL_CALL(glBindVertexArray, m_vao);

        CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 6);

        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);

        CHECKED_GL_CALL(glEnable, GL_DEPTH_TEST);
    }

    void PostProcessing::resize(int widht, int height) {
        if (width <= 0 || height <= 0) {
            return;
        }

        m_width  = widht;
        m_height = height;

        if (m_fbo != 0) {
            CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_fbo);
        }

        if (m_color_texture != 0) {
            CHECKED_GL_CALL(glDeleteTextures, 1, &m_color_texture);
        }

        if (m_depth_stencil_rbo != 0) {
            CHECKED_GL_CALL(glDeleteRenderbuffers, 1, &m_depth_stencil_rbo);
        }

        m_fbo               = 0;
        m_color_texture     = 0;
        m_depth_stencil_rbo = 0;

        create_framebuffer();
    }

    void PostProcessing::destroy() {
        if (m_vao != 0) {
            CHECKED_GL_CALL(glDeleteVertexArrays, 1, &m_vao);
            m_vao = 0;
        }

        if (m_vbo != 0) {
            CHECKED_GL_CALL(glDeleteBuffers, 1, &m_vbo);
            m_vbo = 0;
        }

        if (m_fbo != 0) {
            CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_fbo);
            m_fbo = 0;
        }

        if (m_color_texture != 0) {
            CHECKED_GL_CALL(glDeleteTextures, 1, &m_color_texture);
            m_color_texture = 0;
        }

        if (m_depth_stencil_rbo != 0) {
            CHECKED_GL_CALL(glDeleteRenderbuffers, 1, &m_depth_stencil_rbo);
            m_depth_stencil_rbo = 0;
        }
    }
}
