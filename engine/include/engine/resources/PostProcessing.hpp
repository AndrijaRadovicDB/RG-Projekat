//
// Created by andrija on 18. 9. 2026..
//

#ifndef MATF_RG_PROJECT_POSTPROCESSING_HPP
#define MATF_RG_PROJECT_POSTPROCESSING_HPP

#include <cstdint>

namespace engine::resources {
    class Shader;

    class PostProcessing {
    private:
        void create_framebuffer();

        void destroy();

        int m_width;
        int m_height;

        uint32_t m_fbo{0};
        uint32_t m_color_texture{0};
        uint32_t m_depth_stencil_rbo{0};

        uint32_t m_vao{0};
        uint32_t m_vbo{0};

    public:
        PostProcessing(int width, int height);

        ~PostProcessing();

        PostProcessing(const PostProcessing &) = delete;

        PostProcessing &operator=(const PostProcessing &) = delete;

        void begin();

        void end(Shader *shader);

        void resize(int widht, int height);

        uint32_t framebuffer() const {
            return m_fbo;
        }

        uint32_t color_texture() const {
            return m_color_texture;
        }
    };
}

#endif //MATF_RG_PROJECT_POSTPROCESSING_HPP
