#pragma once
//
// Created by anuding on 2019/11/24.
//
#include <vector>
#include <vulkan/vulkan.h>
#include <glm/detail/type_mat2x2.hpp>
#include "VkComp.h"
namespace Engine::RenderCore {
        struct UBO_MVP{
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };

        class Shader : public VkComp<VkShaderModule> {
        public:
            Shader() = default;
            ~Shader();

            virtual void init(Application* app) override;
            void load(const char* path);
        private:
            void createShaderModule(const std::vector<char>& code);
        };
}